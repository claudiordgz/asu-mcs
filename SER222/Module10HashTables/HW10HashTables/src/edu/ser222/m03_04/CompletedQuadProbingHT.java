package edu.ser222.m03_04;

/**
 * A symbol table implemented using a hashtable with quadratic probing.
 * 
 * @author (put your name here), Acuna
 */
public class CompletedQuadProbingHT<Key, Value> extends CompletedLinearProbingHT<Key, Value> {


    //any constructors must be made public

    //TODO
    public CompletedQuadProbingHT()
    {
        this(997);
    }
    public CompletedQuadProbingHT(int M)
    {
        super(M);
    }
    
    @Override
    public int hash (Key key, int i)
    {
        int h = (((key.hashCode() &0x7fffffff) + i*i) % M);
        return Math.max(h, 0);
    }
}