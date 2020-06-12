#!/usr/bin/python2.7
#
# Interface for the assignement
#

import psycopg2
import math

insert_query_fn = lambda table_name, rows: """
        INSERT INTO {table_name}(UserID, MovieID, Rating)
        VALUES
        {items};
    """.format(table_name=table_name, items=",".join(rows))

single_item_fn = lambda user_id, movie_id, rating: "({}, {}, {})".format(user_id, movie_id, rating)

row_range_by_rating_fn = lambda bin_size, rating: int(math.ceil(max(0, (float(rating) / float(bin_size)) - 1)))


def getOpenConnection(user='postgres', password='1234', dbname='postgres'):
    return psycopg2.connect("dbname='" + dbname + "' user='" + user + "' host='localhost' password='" + password + "'")


def loadRatings(ratingstablename, ratingsfilepath, openconnection):
    cursor = openconnection.cursor()
    cursor.execute("""
    DROP TABLE IF EXISTS ratings;
    CREATE TABLE ratings(
        UserID INTEGER,
        MovieID INTEGER,
        Rating NUMERIC CHECK (rating <= 5)
    );
    """)
    line_to_elements_fn = lambda line: line.split("::")

    with open(ratingsfilepath) as input_data:
        max_per_chunk = 5000
        chunk = []
        for line in input_data:
            if "::" in line:
                user_id, movie_id, rating, _ = line_to_elements_fn(line)
                insert_values = single_item_fn(user_id, movie_id, rating)
                if len(chunk) == max_per_chunk:
                    insert_query_str = insert_query_fn("ratings", chunk)
                    cursor.execute(insert_query_str)
                    chunk = [insert_values]
                else:
                    chunk.append(insert_values)
        if len(chunk) != 0:
            insert_query_str = insert_query_fn("ratings", chunk)
            cursor.execute(insert_query_str)


def rangePartition(ratingstablename, numberofpartitions, openconnection):
    cursor = openconnection.cursor()

    max_range = 5.0
    bin_size = max_range / numberofpartitions

    for i in range(numberofpartitions):
        partition_name = "range_part{}".format(i)
        cursor.execute("""
        DROP TABLE IF EXISTS {partition_name};
        CREATE TABLE {partition_name}(
            UserID INTEGER,
            MovieID INTEGER,
            Rating NUMERIC CHECK (rating <= 5)
        );
        """.format(partition_name=partition_name))

    cursor.itersize = 1000
    query = 'SELECT * FROM ratings ORDER BY Rating, UserID, MovieID;'
    cursor.execute(query)

    for (user_id, movie_id, rating) in cursor:
        rangeinsert(ratingstablename, user_id, movie_id, rating, openconnection, bin_size)


def roundRobinPartition(ratingstablename, numberofpartitions, openconnection):
    cursor = openconnection.cursor()

    for i in range(numberofpartitions):
        partition_name = "rrobin_part{}".format(i)
        cursor.execute("""
        DROP TABLE IF EXISTS {partition_name};
        CREATE TABLE {partition_name}(
            UserID INTEGER,
            MovieID INTEGER,
            Rating NUMERIC CHECK (rating <= 5)
        );
        """.format(partition_name=partition_name))

    cursor.itersize = 1000
    query = 'SELECT * FROM ratings ORDER BY Rating, UserID, MovieID;'
    cursor.execute(query)

    for (user_id, movie_id, rating) in cursor:
        roundrobininsert(ratingstablename, user_id, movie_id, rating, openconnection, numberofpartitions)


def roundrobininsert(ratingstablename, userid, movie_id, rating, openconnection, number_of_partitions=5):
    partitions = getattr(roundrobininsert, 'number_of_partitions', number_of_partitions)
    current_partition = getattr(roundrobininsert, 'current_partition', 0)
    
    partition_name = "rrobin_part{}".format(current_partition)
    insert_values = single_item_fn(userid, movie_id, rating)
    insert_query_str = insert_query_fn(partition_name, [insert_values])
    
    inner_cursor = openconnection.cursor()
    inner_cursor.execute(insert_query_str)

    roundrobininsert.current_partition = current_partition + 1 if current_partition + 1 < partitions else 0

def rangeinsert(ratingstablename, userid, movie_id, rating, openconnection, bin_size=1):
    inner_cursor = openconnection.cursor()
    partition_number = row_range_by_rating_fn(bin_size, rating)
    partition_name = "range_part{}".format(partition_number)
    insert_values = single_item_fn(userid, movie_id, rating)
    insert_query_str = insert_query_fn(partition_name, [insert_values])
    inner_cursor.execute(insert_query_str)

def createDB(dbname='dds_assignment'):
    """
    We create a DB by connecting to the default user and database of Postgres
    The function first checks if an existing database exists for a given name, else creates it.
    :return:None
    """
    # Connect to the default database
    con = getOpenConnection(dbname='postgres')
    con.set_isolation_level(psycopg2.extensions.ISOLATION_LEVEL_AUTOCOMMIT)
    cur = con.cursor()

    # Check if an existing database with the same name exists
    cur.execute('SELECT COUNT(*) FROM pg_catalog.pg_database WHERE datname=\'%s\'' % (dbname,))
    count = cur.fetchone()[0]
    if count == 0:
        cur.execute('CREATE DATABASE %s' % (dbname,))  # Create the database
    else:
        print 'A database named {0} already exists'.format(dbname)

    # Clean up
    cur.close()
    con.close()

def deletepartitionsandexit(openconnection):
    cur = openconnection.cursor()
    cur.execute("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'")
    l = []
    for row in cur:
        l.append(row[0])
    for tablename in l:
        cur.execute("drop table if exists {0} CASCADE".format(tablename))

    cur.close()

def deleteTables(ratingstablename, openconnection):
    try:
        cursor = openconnection.cursor()
        if ratingstablename.upper() == 'ALL':
            cursor.execute("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'")
            tables = cursor.fetchall()
            for table_name in tables:
                cursor.execute('DROP TABLE %s CASCADE' % (table_name[0]))
        else:
            cursor.execute('DROP TABLE %s CASCADE' % (ratingstablename))
        openconnection.commit()
    except psycopg2.DatabaseError, e:
        if openconnection:
            openconnection.rollback()
        print 'Error %s' % e
    except IOError, e:
        if openconnection:
            openconnection.rollback()
        print 'Error %s' % e
    finally:
        if cursor:
            cursor.close()
