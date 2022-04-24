package edu.ser222.m03_04;

/**
 * A symbol table implemented using a hashtable with chaining.
 * Does not support load balancing or resizing.
 * 
 * @author (put your name here), Sedgewick and Wayne, Acuna
 */
import java.util.LinkedList;
import java.util.Queue;

public class CompletedTwoProbeChainHT<Key, Value> implements TwoProbeChainHT<Key, Value> {

    //any constructors must be made public

    @Override
    public int hash(Key key) {
        //TODO
        return 0;
    }

    @Override
    public int hash2(Key key) {
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
    // THESE METHODS ARE ONLY FOR GRADING AND COME FROM THE TWOPROBECHAINHT INTERFACE.

    @Override
    public int getM() {
        //TODO. We suggest something like:
        //return M;

        return 0;
    }

    @Override
    public int getChainSize(int i) {
        //TODO. We suggest something like:
        //return entries[i].size();

        return 0;
    }
}