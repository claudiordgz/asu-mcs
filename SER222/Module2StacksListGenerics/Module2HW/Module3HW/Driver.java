package edu.ser222.m01_03;

/**
 * Test area for list implementations.
 * 
 * @author (your name), Acuna
 * @version (version)
 */
public class Driver {
    public static void main(String [] args) {
        //ListADT<Integer> list = new CompletedList<>(); //simpler version
        OrderedListADT<Integer> list = new CompletedOrderedList<>();
        
        //RA: These are _extremely_ simple tests! You will need to write more!
        
        list.add(23);
        list.add(24);	
        list.add(16);
        list.add(3);	
        list.add(7);
        list.add(17);	
        list.add(9);	
        list.add(13);	
        list.add(14);	
        list.add(1);

        System.out.println(list);
        
        list.remove(7);
        list.removeFirst();
        list.remove(17);
        list.removeLast();
        list.remove(14);
        list.removeLast();
        
        System.out.println(list);
        
        /* Test Results:
            1 3 7 9 13 14 16 17 23 24 
            3 9 13 16 
        */
    }
}