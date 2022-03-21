package edu.ser222.m01_03;

import java.util.NoSuchElementException;
import java.util.Objects;

/**
 * This program provides an implementation of the Deque interface. Also provides a main that
 * demonstrates it.
 * 
 * @author (your name), Acuna
 * @version (version)
 */

public class CompletedDeque<Item> implements Deque<Item> {

    static private class DoubleLinearNode<Item> {

        private Item data;
        private DoubleLinearNode<Item> nextNode;
        private DoubleLinearNode<Item> prevNode;

        public DoubleLinearNode(Item data){
            this.data = data;
            this.nextNode = null;
            this.prevNode = null;
        }

        public Item getData() {
            return data;
        }

        public DoubleLinearNode<Item> getNextNode() {
            return nextNode;
        }

        public DoubleLinearNode<Item> getPrevNode() {
            return prevNode;
        }

        public void setNextNode(DoubleLinearNode<Item> nextNode) {
            this.nextNode = nextNode;
        }

        public void setPrevNode(DoubleLinearNode<Item> prevNode) {
            this.prevNode = prevNode;
        }
    }

    private DoubleLinearNode<Item> head;
    private DoubleLinearNode<Item> tail;
    private int size;


    public CompletedDeque() {
        size = 0;
    }

    @Override
    public void enqueueFront(Item element) {
        DoubleLinearNode<Item> newNode = new DoubleLinearNode<Item>(element);
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.setNextNode(head);
            head.setPrevNode(newNode);
            head = newNode;
        }
        size++;
    }

    @Override
    public void enqueueBack(Item element) {
        DoubleLinearNode<Item> newNode = new DoubleLinearNode<Item>(element);
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
        } else {
            newNode.setPrevNode(tail);
            tail.setNextNode(newNode);
            tail = newNode;
        }
        size++;
    }

    @Override
    public Item dequeueFront() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        Item item = head.getData();
        head = head.getNextNode();
        size--;
        if (isEmpty()) {
            tail = null;
        } else {
            head.setPrevNode(null);
        }
        return item;
    }

    @Override
    public Item dequeueBack() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        Item item = tail.getData();
        tail = tail.getPrevNode();
        size--;
        if (isEmpty()) {
            head = null;
        } else {
            tail.setNextNode(null);
        }
        return item;
    }

    @Override
    public Item first() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        return head.getData();
    }

    @Override
    public Item last() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        return tail.getData();
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        if (size == 0) {
            sb.append("empty");
            return sb.toString();
        }
        DoubleLinearNode<Item> current = tail;
        while (current != null) {
            sb.append(current.getData());
            current = current.getPrevNode();
            if (current != null) {
                sb.append(" ");
            }
        }
        return sb.toString();
    }

    public String contentsFromTop() {
        StringBuilder sb = new StringBuilder();
        if (size == 0) {
            sb.append("empty");
            return sb.toString();
        }
        DoubleLinearNode<Item> current = head;
        while (current != null) {
            sb.append(current.getData());
            current = current.getNextNode();
            if (current != null) {
                sb.append(" ");
            }
        }
        return sb.toString();
    }

    public static final String COLOR_RESET = "\u001B[0m";
    public static final String COLOR_RED = "\u001B[31m";
    public static final String COLOR_GREEN = "\u001B[32m";

    public static <T> void assertEquals(T result, T expected, String name) {
        if (!Objects.equals(result, expected)) {
            System.out.println("[" + name + "]:" + COLOR_RED + "FAIL" + COLOR_RESET);
            System.out.println("\t[Received]:" + COLOR_RED + result + COLOR_RESET);
            System.out.println("\t[Expected]:" + COLOR_RED + expected + COLOR_RESET);
        } else {
            System.out.println("[" + name + "]:" + COLOR_GREEN + "PASS" + COLOR_RESET);
            System.out.println("\t[Received]:" + COLOR_GREEN + result + COLOR_RESET);
            System.out.println("\t[Expected]:" + COLOR_GREEN + expected + COLOR_RESET);
        }
    }
    
    public static void testIntegers() {
        CompletedDeque<Integer> deque = new CompletedDeque<>();

        //standard queue behavior
        deque.enqueueBack(3);
        deque.enqueueBack(7);
        deque.enqueueBack(4);
        Integer item = deque.dequeueFront();
        assertEquals(item, 3, "Expected front");
        deque.enqueueBack(9);
        deque.enqueueBack(8);
        item = deque.dequeueFront();
        assertEquals(item, 7, "Expected front");
        assertEquals(deque.size(), 3, "Expected Size");
        String strRep = deque.toString();
        assertEquals(strRep, "8 9 4", "String order");

        //deque features
        System.out.println(deque.dequeueFront());
        deque.enqueueFront(1);
        deque.enqueueFront(11);                         
        deque.enqueueFront(3);                 
        deque.enqueueFront(5);
        System.out.println(deque.dequeueBack());
        System.out.println(deque.dequeueBack());        
        System.out.println(deque.last());                
        deque.dequeueFront();
        deque.dequeueFront();        
        System.out.println(deque.first());        
        System.out.println("size: " + deque.size());
        strRep = deque.toString();
        assertEquals(strRep, "1 11", "String order");
    }
    
    public static void testStrings() {
        CompletedDeque<String> deque = new CompletedDeque<>();

        //standard queue behavior
        deque.enqueueBack("3");
        deque.enqueueBack("7");
        deque.enqueueBack("4");
        deque.dequeueFront();        
        deque.enqueueBack("9");
        deque.enqueueBack("8");
        deque.dequeueFront();
        System.out.println("size: " + deque.size());
        String strRep = deque.toString();
        System.out.println("contents:\n" + strRep);
        assertEquals(strRep, "8 9 4", "String order");

        //deque features
        System.out.println(deque.dequeueFront());        
        deque.enqueueFront("1");
        deque.enqueueFront("11");                         
        deque.enqueueFront("3");                 
        deque.enqueueFront("5");
        System.out.println(deque.dequeueBack());
        System.out.println(deque.dequeueBack());
        System.out.println(deque.last());
        deque.dequeueFront();
        deque.dequeueFront();
        System.out.println(deque.first());
        System.out.println("size: " + deque.size());
        strRep = deque.toString();
        System.out.println("contents:\n" + strRep);
        assertEquals(strRep, "1 11", "String order");
    }

    /**
     * Program entry point for deque. 
     * @param args command line arguments
     */    
    public static void main(String[] args) {
        testIntegers();
        testStrings();
    }
}