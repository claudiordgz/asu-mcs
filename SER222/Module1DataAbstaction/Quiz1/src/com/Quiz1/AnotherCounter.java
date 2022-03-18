package com.Quiz1;

public class AnotherCounter implements IncrementCounter {

    private String ID;

    public AnotherCounter(String param) {
        this.ID = param;
    }

    @Override
    public void increment() {
        ID = Integer.toString(Integer.parseInt(this.ID)+1);
    }

    @Override
    public int tally() {
        return Integer.parseInt(this.ID);
    }

    @Override
    public String toString() {
        return "ID=[" + this.ID + ']';
    }
}
