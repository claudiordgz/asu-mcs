#!/usr/bin/python2.7
#
# Assignment3 Interface
#

import psycopg2
import os
import sys

# Donot close the connection inside this file i.e. do not perform openconnection.close()
def ParallelSort (InputTable, SortingColumnName, OutputTable, openconnection):
    cursor = openconnection.cursor()
    cursor.execute("""
        DROP TABLE IF EXISTS {output_table};
        CREATE TABLE {output_table} AS
        SELECT *
        FROM {table_name}
        ORDER BY {sorting_column};
    """.format(
        table_name=InputTable,
        sorting_column=SortingColumnName,
        output_table=OutputTable
    )) 

def ParallelJoin (InputTable1, InputTable2, Table1JoinColumn, Table2JoinColumn, OutputTable, openconnection):
    cursor = openconnection.cursor()
    parallel_join_query = """
        DROP TABLE IF EXISTS {output_table};
        CREATE TABLE {output_table} AS
        SELECT *
        FROM {table1}
        JOIN {table2}
        ON {table1}.{key1} = {table2}.{key2};
    """.format(
        table1=InputTable1,
        table2=InputTable2,
        key1=Table1JoinColumn,
        key2=Table2JoinColumn,
        output_table=OutputTable
    )
    cursor.execute(parallel_join_query)

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
