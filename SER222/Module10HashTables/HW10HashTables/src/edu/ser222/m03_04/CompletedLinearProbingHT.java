package edu.ser222.m03_04;

/**
 * A symbol table implemented using a hashtable with linear probing.
 * 
 * @author (put your name here), Sedgewick and Wayne, Acuna
 */
import java.util.LinkedList;

public class CompletedLinearProbingHT<Key, Value> implements ProbingHT<Key, Value> {

    private class KeyValue<Key, Value> {
        public Key key;
        public Value value;

        public KeyValue(Key k, Value v) {
            this.key = k;
            this.value = v;
        }
    }

    //any constructors must be made public
    protected int M;
    private int N;
    private KeyValue[] entries;

    public CompletedLinearProbingHT() {
        this(997);
    }
    public CompletedLinearProbingHT(int M) {
        this.M = M;
        this.N = 0;
        entries = new KeyValue[this.M];
    }

    @Override
    public int hash(Key key, int i) {
        //TODO
        int h =  (((key.hashCode() & 0x7fffffff) + i) % M);
        return Math.max(h, 0);
    }

    @Override
    public void put(Key key, Value val) {
        //TODO
        int idx = 0;
        int h = hash(key, 0);
        while (entries[h] != null) {
            if (entries[h].key.equals(key)) {
                entries[h].value = val;
                return;
            }
            idx += 1;
            h = hash(key, idx);
        }
        entries[h] = new KeyValue(key, val);
        N++;
    }



    @Override
    public Value get(Key key) {
        //TODO
        int idx = 0;
        int h = hash(key, idx);
        while (entries[h] != null) {
            if (entries[h].key.hashCode() == key.hashCode()) {
                return (Value) entries[h].value;
            }
            idx += 1;
            h = hash(key, idx);
        }
        return null;
    }

    @Override
    public void delete(Key key) {
        //TODO
        int idx = 0;
        int h = hash(key, idx);
        while (entries[h] != null) {
            if (entries[h].key.hashCode() == key.hashCode()) {
                int idx2 = idx + 1;
                int h2 = hash(key, idx2);
                while (entries[h2] != null) {
                    entries[h] = entries[h2];
                    idx += 1;
                    idx2 += 1;
                    h = hash(key, idx);
                    h2 = hash(key, idx2);
                }
                entries[h] = null;
                N--;
            }
            idx += 1;
            h = hash(key, idx);
        }
    }

    @Override
    public boolean contains(Key key) {
        //TODO
        int idx = 0;
        int h = hash(key, idx);
        while (entries[h] != null) {
            if (entries[h].key.hashCode() == key.hashCode()) {
                return true;
            }
            idx += 1;
            h = hash(key, idx);
        }
        return false;
    }

    @Override
    public boolean isEmpty() {
        //TODO
        return size() == 0;
    }

    @Override
    public int size() {
        //TODO
        return N;
    }

    @Override
    public Iterable<Key> keys() {
        //TODO
        LinkedList<Key> keys = new LinkedList<>();
        for (KeyValue entry : entries) {
            if (entry != null) {
                keys.add((Key) entry.key);
            }
        }
        return keys;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // THESE METHODS ARE ONLY FOR GRADING AND COME FROM THE PROBINGHT INTERFACE.

    @Override
    public int getM() {
        //TODO. We suggest something like:
        return M;
    }

    @Override
    public Object getTableEntry(int i) {
        //TODO. We suggest something like:
        return entries[i];
    }
}