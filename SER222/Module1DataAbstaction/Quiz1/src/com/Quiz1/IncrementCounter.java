package com.Quiz1;

public interface IncrementCounter {

    //Increments the counter by one.

    void increment();

    //Returns the number of increments since creation.

    int tally();

    //Returns a string representation that counts number of increments and the ID of the counter.

    String toString();

}

