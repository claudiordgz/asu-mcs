#!/usr/bin/python2.7
#
# Assignment3 Interface
#

import psycopg2
import os
import sys
from threading import Thread
from itertools import chain, imap

insert_query_fn = lambda table_name, rows: """
        INSERT INTO {table_name}(UserID, MovieID, Rating)
        VALUES
        {items};
    """.format(table_name=table_name, items=",".join(rows))

single_item_fn = lambda user_id, movie_id, rating: "({}, {}, {})".format(user_id, movie_id, rating)

def get_count(cursor, table_name):
    cursor.execute("""
        SELECT COUNT(*) FROM {}
    """.format(table_name))
    count = cursor.fetchone()[0]
    return count

class GetItemsThread(Thread):

    def __init__(self, openconnection, query):
        self.openconnection = openconnection
        self.query = query
        self.rows = []
        super(GetItemsThread, self).__init__()

    def run(self):
        cursor = self.openconnection.cursor()
        cursor.execute(self.query)
        self.rows = cursor.fetchall()

def make_query(input_table, offset, chunk_size):
    chunk_query = """
        SELECT *
        FROM {input_table}
        OFFSET {offset}
        LIMIT {chunk_size};
    """.format(input_table=input_table, offset=offset, chunk_size=chunk_size)
    return chunk_query

def make_parallel_join_query(input_table1, input_table2, key1, key2, offset, chunk_size):
    chunk_query = """
        SELECT *
        FROM {input_table1}
        JOIN {input_table2}
        ON {input_table1}.{key1} = {input_table2}.{key2}
        OFFSET {offset}
        LIMIT {chunk_size};
    """.format(input_table1=input_table1, input_table2=input_table2, key1=key1, key2=key2, offset=offset, chunk_size=chunk_size)
    return chunk_query

def flatmap(items):
    return chain.from_iterable(items)

# Donot close the connection inside this file i.e. do not perform openconnection.close()
def ParallelSort (InputTable, SortingColumnName, OutputTable, openconnection):
    cursor = openconnection.cursor()
    cursor.execute("""
        DROP TABLE IF EXISTS {output_table};
        CREATE TABLE {output_table} AS
        SELECT *
        FROM {input_table}
        LIMIT 0;
    """.format(input_table=InputTable, output_table=OutputTable))
    count = get_count(cursor, InputTable)
    chunk_size = count // 5
    offsets = range(0, count, chunk_size)
    queries = map(lambda offset: make_query(InputTable, offset, chunk_size), offsets)
    threads = [GetItemsThread(openconnection, query) for query in queries]
    for thread in threads:
        thread.start()
        thread.join()
    rows = map(lambda row: single_item_fn(row[0], row[1], row[2]), sorted(flatmap(map(lambda t: t.rows, threads)), key=lambda row: row[2]))
    cursor.execute(insert_query_fn(OutputTable, rows))


def ParallelJoin (InputTable1, InputTable2, Table1JoinColumn, Table2JoinColumn, OutputTable, openconnection):
    cursor = openconnection.cursor()
    parallel_join_query = """
        DROP TABLE IF EXISTS {output_table};
        CREATE TABLE {output_table} AS
        SELECT *
        FROM {table1}
        JOIN {table2}
        ON {table1}.{key1} = {table2}.{key2}
        LIMIT 0;
    """.format(
        table1=InputTable1,
        table2=InputTable2,
        key1=Table1JoinColumn,
        key2=Table2JoinColumn,
        output_table=OutputTable
    )
    cursor.execute(parallel_join_query)
    cursor.execute("""
        SELECT COUNT(*)
        FROM {table1}
        JOIN {table2}
        ON {table1}.{key1} = {table2}.{key2};
    """.format(
        table1=InputTable1,
        table2=InputTable2,
        key1=Table1JoinColumn,
        key2=Table2JoinColumn,
        output_table=OutputTable
    ))
    count = cursor.fetchone()[0]
    chunk_size = count // 5
    offsets = range(0, count, chunk_size)
    queries = map(lambda offset: make_parallel_join_query(InputTable1, InputTable2, Table1JoinColumn, Table2JoinColumn, offset, chunk_size), offsets)
    threads = [GetItemsThread(openconnection, query) for query in queries]
    for thread in threads:
        thread.start()
        thread.join()
    rows = flatmap(map(lambda t: t.rows, threads))
    
    cursor.execute("""                              
        SELECT *
        FROM {}
        LIMIT 0;
    """.format(OutputTable))
    columns = "({})".format(",".join(map(lambda column_arr: column_arr.name, cursor.description)))
    
    rows = map(lambda row: "({}, {}, {}, {}, '{}', '{}')".format(row[0], row[1], row[2], row[3], row[4], row[5]), rows)
    cursor.execute("""
        INSERT INTO {table_name}{columns}
        VALUES
        {items};
    """.format(table_name=OutputTable, columns=columns, items=",".join(rows)))


################### DO NOT CHANGE ANYTHING BELOW THIS #############################


# Donot change this function
def getOpenConnection(user='postgres', password='1234', dbname='postgres'):
    return psycopg2.connect("dbname='" + dbname + "' user='" + user + "' host='localhost' password='" + password + "'")

# Donot change this function
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
    con.commit()
    con.close()
