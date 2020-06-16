#!/usr/bin/python2.7
#
# Assignment2 Interface
#

import psycopg2
import os
import sys
# Donot close the connection inside this file i.e. do not perform openconnection.close()
def RangeQuery(ratingsTableName, ratingMinValue, ratingMaxValue, openconnection):
    cursor = openconnection.cursor()
    cursor.execute("""
        SELECT 'RangeRatingsPart0' as Partition, *
        FROM rangeratingspart0
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RangeRatingsPart1' as Partition, *
        FROM rangeratingspart1
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RangeRatingsPart2' as Partition, *
        FROM rangeratingspart2
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RangeRatingsPart3' as Partition, *
        FROM rangeratingspart3
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RangeRatingsPart4' as Partition, *
        FROM rangeratingspart4
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RoundRobinRatingsPart0' as Partition, *
        FROM roundrobinratingspart0
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RoundRobinRatingsPart1' as Partition, *
        FROM roundrobinratingspart1
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RoundRobinRatingsPart2' as Partition, *
        FROM roundrobinratingspart2
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RoundRobinRatingsPart3' as Partition, *
        FROM roundrobinratingspart3
        WHERE rating BETWEEN {min_rating} AND {max_rating}
        UNION
        SELECT 'RoundRobinRatingsPart4' as Partition, *
        FROM roundrobinratingspart4
        WHERE rating BETWEEN {min_rating} AND {max_rating};
    """.format(min_rating=ratingMinValue, max_rating=ratingMaxValue))
    writeToFile("RangeQueryOut.txt", cursor.fetchall())

def PointQuery(ratingsTableName, ratingValue, openconnection):
    cursor = openconnection.cursor()
    cursor.execute("""
        SELECT 'RangeRatingsPart0' as Partition, *
        FROM rangeratingspart0
        WHERE rating = {rating}
        UNION
        SELECT 'RangeRatingsPart1' as Partition, *
        FROM rangeratingspart1
        WHERE rating = {rating}
        UNION
        SELECT 'RangeRatingsPart2' as Partition, *
        FROM rangeratingspart2
        WHERE rating = {rating}
        UNION
        SELECT 'RangeRatingsPart3' as Partition, *
        FROM rangeratingspart3
        WHERE rating = {rating}
        UNION
        SELECT 'RangeRatingsPart4' as Partition, *
        FROM rangeratingspart4
        WHERE rating = {rating}
        UNION
        SELECT 'RoundRobinRatingsPart0' as Partition, *
        FROM roundrobinratingspart0
        WHERE rating = {rating}
        UNION
        SELECT 'RoundRobinRatingsPart1' as Partition, *
        FROM roundrobinratingspart1
        WHERE rating = {rating}
        UNION
        SELECT 'RoundRobinRatingsPart2' as Partition, *
        FROM roundrobinratingspart2
        WHERE rating = {rating}
        UNION
        SELECT 'RoundRobinRatingsPart3' as Partition, *
        FROM roundrobinratingspart3
        WHERE rating = {rating}
        UNION
        SELECT 'RoundRobinRatingsPart4' as Partition, *
        FROM roundrobinratingspart4
        WHERE rating = {rating};
    """.format(rating=ratingValue))
    writeToFile("PointQueryOut.txt", cursor.fetchall())


def writeToFile(filename, rows):
    f = open(filename, 'w')
    for line in rows:
        f.write(','.join(str(s) for s in line))
        f.write('\n')
    f.close()
