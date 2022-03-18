package com.Quiz1;


public class Main {

    public static void distanceBetweenPoints(Point2D a, Point2D b) {
        String distance = String.valueOf(a.distanceTo(b));
        String sentence = "The distance between (" + a.x() + "," + a.y() +") and (" + b.x() + "," + b.y() + ") is " + distance + ".";
        System.out.println(sentence);
    }

    public static void main(String[] args) {
        AnotherCounter a = new AnotherCounter("1");
        a.increment();
        System.out.println(a);
        a.increment();
        System.out.println(a);
        a.increment();
        System.out.println(a);
    }
}
