package edu.ser222.m03_04;

/**
 * A symbol table implemented using a hashtable with chaining.
 * Does not support load balancing or resizing.
 * 
 * @author (put your name here), Sedgewick and Wayne, Acuna
 */
import java.util.LinkedList;

public class CompletedTwoProbeChainHT<Key, Value> implements TwoProbeChainHT<Key, Value> {

    private class KeyValue {
        public Key key;
        public Value value;

        public KeyValue(Key k, Value v) {
            this.key = k;
            this.value = v;
        }
    }

    private final int M;
    private int N;
    private LinkedList<KeyValue>[] entries;

    //any constructors must be made public

    public CompletedTwoProbeChainHT() {
        this(997);
    }
    public CompletedTwoProbeChainHT(int M) {
        this.M = M;
        this.N = 0;
        entries = new LinkedList[this.M];
        for (int i = 0; i != this.M; i++) {
            entries[i] = new LinkedList<>();
        }
    }

    @Override
    public int hash(Key key) {
        return (key.hashCode() & 0x7fffffff) % M;
    }

    @Override
    public int hash2(Key key) {
        return (((key.hashCode() & 0x7fffffff) % M) * 31) % M;
    }

    @Override
    public void put(Key key, Value val) {
        int idxH1 = hash(key);
        int idxH2 = hash2(key);

        LinkedList<KeyValue> listH1 = entries[idxH1];
        LinkedList<KeyValue> listH2 = entries[idxH2];
        for (KeyValue entry : listH1) {
            if (entry.key.hashCode() == key.hashCode()) {
                entry.value = val;
                return;
            }
        }
        for (KeyValue entry : listH2) {
            if (entry.key.hashCode() == key.hashCode()) {
                entry.value = val;
                return;
            }
        }

        KeyValue newEntry = new KeyValue(key, val);
        if (listH1.size() < listH2.size()) {
            listH1.addLast(newEntry);
        } else {
            listH2.addLast(newEntry);
        }
        
        N++;
    }

    @Override
    public Value get(Key key) {
        int idxH1 = hash(key);
        int idxH2 = hash2(key);

        LinkedList<KeyValue> listH1 = entries[idxH1];
        LinkedList<KeyValue> listH2 = entries[idxH2];

        for (KeyValue entry : listH1) {
            if (entry.key.hashCode() == key.hashCode()) {
                return entry.value;
            }
        }
        for (KeyValue entry : listH2) {
            if (entry.key.hashCode() == key.hashCode()) {
                return entry.value;
            }
        }
        return null;
    }

    @Override
    public void delete(Key key) {
        if (key == null) throw new UnsupportedOperationException();

        int idxH1 = hash(key);
        int idxH2 = hash2(key);

        LinkedList<KeyValue> listH1 = entries[idxH1];
        LinkedList<KeyValue> listH2 = entries[idxH2];

        for (KeyValue entry : listH1) {
            if (entry.key.hashCode() == key.hashCode()) {
                listH1.removeFirstOccurrence(entry);
                N--;
                return;
            }
        }
        for (KeyValue entry : listH2) {
            if (entry.key.hashCode() == key.hashCode()) {
                listH2.removeFirstOccurrence(entry);
                N--;
                return;
            }
        }
    }

    @Override
    public boolean contains(Key key) {
        if (key == null) throw new UnsupportedOperationException();
        return get(key) != null;
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
        LinkedList<Key> keys = new LinkedList();
        for (int i = 0; i < M; i++)
        {
            for (KeyValue e : entries[i])
            {
                keys.add(e.key);
            }
        }
        return keys;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // THESE METHODS ARE ONLY FOR GRADING AND COME FROM THE TWOPROBECHAINHT INTERFACE.

    @Override
    public int getM() {
        //TODO. We suggest something like:
        return M;
    }

    @Override
    public int getChainSize(int i) {
        //TODO. We suggest something like:
        return entries[i].size();
    }
}