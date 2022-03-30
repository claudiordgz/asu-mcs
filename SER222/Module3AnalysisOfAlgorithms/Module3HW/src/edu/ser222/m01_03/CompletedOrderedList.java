package edu.ser222.m01_03;

/**
 * CompletedOrderedList represents an implementation of an ordered list that builds on
 * CompletedList.
 *
 * @author Claudio Rodriguez Rodriguez, Acuna
 * @version 3/16/2022
 */
public class CompletedOrderedList<T extends Comparable<T>> extends CompletedList<T>
         implements OrderedListADT<T> {

    @Override
    public void add(T element) throws NullPointerException {
        if (element == null) {
            throw new NullPointerException("Element cannot be null");
        }
        if (isEmpty()) {
            head = new DoubleLinearNode<T>(element);
            tail = head;
        } else {
            DoubleLinearNode<T> current = head;
            DoubleLinearNode<T> newNode = new DoubleLinearNode<T>(element);
            while (current != null && current.getElement().compareTo(element) < 0) {
                current = current.getNext();
            }
            // it's the end of the list
            if (current == null) {
                newNode.setPrevious(tail);
                tail.setNext(newNode);
                tail = tail.getNext();
            } 
            // it's the beginning of the list
            else if (current.getPrevious() == null) {
                newNode.setNext(head);
                head.setPrevious(newNode);
                head = newNode;
            } 
            // it's in the middle of the list
            else {
                newNode.setNext(current);
                newNode.setPrevious(current.getPrevious());
                current.getPrevious().setNext(newNode);
                current.setPrevious(newNode);
            }
        }
        n++;
        modCount++;
    }

}
