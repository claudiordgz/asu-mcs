package edu.ser222.m01_03;

import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * CompletedList represents an implementation of a list.
 *
 * @author Claudio Rodriguez Rodriguez, Acuna
 *  * @version 3/16/2022
 */
public class CompletedList<T> implements ListADT<T>, Iterable<T> {

    //The following three variables are a suggested start if you are using a list implementation.
    protected int n = 0;
    protected int modCount = 0;
    protected DoubleLinearNode<T> head; 
    protected DoubleLinearNode<T> tail;

    public CompletedList() {
        head = null;
        tail = null;
    }

    public void add(T element) throws NullPointerException {
        if (element == null) {
            throw new NullPointerException("Element cannot be null");
        }
        if (isEmpty()) {
            head = new DoubleLinearNode<T>(element);
            tail = head;
        } else {
            DoubleLinearNode<T> newNode = new DoubleLinearNode<T>(element);
            tail.setNext(newNode);
            newNode.setPrevious(tail);
            tail = newNode;
        }
        n++;
        modCount++;
    } 

    @Override
    public T removeFirst() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        T element = head.getElement();
        head = head.getNext();
        n--;
        modCount++;
        return element;
    }

    @Override
    public T removeLast() throws NoSuchElementException {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        T element = tail.getElement();
        tail = tail.getPrevious();
        tail.setNext(null);
        n--;
        modCount++;
        return element;
    }

    @Override
    public T remove(T element) {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        for (DoubleLinearNode<T> current = head; current != null; current = current.getNext()) {
            if (current.getElement().equals(element)) {
                T currentElement = current.getElement();
                DoubleLinearNode<T> next = current.getNext();
                DoubleLinearNode<T> previous = current.getPrevious();
                // it's the only element
                if (previous == null && next == null) {
                    head = null;
                    tail = null;
                }
                // it's the head
                else if (previous == null) {
                    head = next;
                    next.setPrevious(null);
                }
                // it's the tail
                else if (next == null) {
                    tail = previous;
                    previous.setNext(null);
                }
                // it's in the middle
                else {
                    previous.setNext(next);
                    next.setPrevious(previous);
                }
                n--;
                modCount++;
                return currentElement;
            }
        }
        throw new NoSuchElementException();
    }

    @Override
    public T first() {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        return head.getElement();
    }

    @Override
    public T last() {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        return tail.getElement();
    }

    @Override
    public boolean contains(T target) {
        if (isEmpty()) {
            throw new NoSuchElementException();
        }
        for (DoubleLinearNode<T> current = head; current != null; current = current.getNext()) {
            if (current.getElement().equals(target)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean isEmpty() {
        return n == 0;
    }

    @Override
    public int size() {
        return n;
    }

    @Override
    public Iterator<T> iterator() {
        return new ListIterator();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (T current: this) {
            if (sb.length() != 0) {
                sb.append(" ");
            }
            sb.append(current);
        }
        return sb.toString();
    }

        
    public class DoubleLinearNode<T> {
        private DoubleLinearNode<T> next;
        private DoubleLinearNode<T> previous;
        private T element;

        public DoubleLinearNode() {
            next = null;
            previous = null;
            element = null;
        }

        public DoubleLinearNode(T elem) {
            next = null;
            previous = null;
            element = elem;
        }

        public DoubleLinearNode<T> getNext() {
            return next;
        }

        public void setNext(DoubleLinearNode<T> node) {
            next = node;
        }

        public void setPrevious(DoubleLinearNode<T> node) {
            previous = node;
        }

        public DoubleLinearNode<T> getPrevious() {
            return previous;
        }

        public T getElement() {
            return element;
        }

        public void setElement(T elem) {
            element = elem;
        }
    }

    private class ListIterator implements Iterator<T> {
            
        private final int modCounted = modCount;
        private DoubleLinearNode<T> iter = head;

        public ListIterator() {
        }

        @Override
        public boolean hasNext() {
            return iter != null;
        }

        @Override
        public T next() {
            if (!hasNext())
                throw new NoSuchElementException();

            if(modCount != modCounted)
                throw new ConcurrentModificationException();

            T element = iter.getElement();
            iter = iter.getNext();

            return element;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }
    } 
}