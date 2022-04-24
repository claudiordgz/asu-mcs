package edu.ser222.m03_04;

/**
 * A symbol table implemented using a hashtable with linear probing.
 * 
 * @author (put your name here), Sedgewick and Wayne, Acuna
 */
import java.util.LinkedList;
import java.util.Queue;

public class CompletedLinearProbingHT<Key, Value> implements ProbingHT<Key, Value> {

    //any constructors must be made public

    @Override
    public int hash(Key key, int i) {
        //TODO
        return 0;
    }

    @Override
    public void put(Key key, Value val) {
        //TODO
    }

    @Override
    public Value get(Key key) {
        //TODO
        return null;
    }

    @Override
    public void delete(Key key) {
        //TODO
    }

    @Override
    public boolean contains(Key key) {
        //TODO
        return false;
    }

    @Override
    public boolean isEmpty() {
        //TODO
        return false;
    }

    @Override
    public int size() {
        //TODO
        return 0;
    }

    @Override
    public Iterable<Key> keys() {
        //TODO
        return null;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // THESE METHODS ARE ONLY FOR GRADING AND COME FROM THE PROBINGHT INTERFACE.

    @Override
    public int getM() {
        //TODO. We suggest something like:
        //return M;

        return 0;
    }

    @Override
    public Object getTableEntry(int i) {
        //TODO. We suggest something like:
        //return entries[i];

        return 0;
    }
}