package edu.ser222.m01_03;

/**
 * Test area for list implementations.
 * 
 * @author (your name), Acuna
 * @version (version)
 */
public class Driver {
    public static void assertEquals(String result, String expected, String message) {
        if (result.equals(expected)) {
            System.out.println("✅ Test: " + message);
        } else {
            System.out.println("❌ Test: " + message);
            System.out.println("Expected: " + expected);
            System.out.println("Actual: " + result);
        }
    }

    public static void testIntegerList() {
        //ListADT<Integer> list = new CompletedList<>(); //simpler version
        OrderedListADT<Integer> list = basicIntegerList();

        //RA: These are _extremely_ simple tests! You will need to write more!
        String result = list.toString();
        String expected = "1 3 7 9 13 14 16 17 23 24";

        assertEquals(result, expected, "operations test - numbers added in order");

        list.remove(7);
        list.removeFirst();
        list.remove(17);
        list.removeLast();
        list.remove(14);
        list.removeLast();

        result = list.toString();
        expected = "3 9 13 16";
        assertEquals(result, expected, "operations test - removed correct elements");
    }

    private static void testStringList() {
        //ListADT<Integer> list = new CompletedList<>(); //simpler version
        OrderedListADT<String> list = new CompletedOrderedList<>();

        //RA: These are _extremely_ simple tests! You will need to write more!
        String result = "";
        String expected = "";

        list.add("23");
        list.add("24");
        list.add("16");
        list.add("3");
        list.add("7");
        list.add("17");
        list.add("9");
        list.add("13");
        list.add("14");
        list.add("1");

        result = list.toString();
        expected = "1 13 14 16 17 23 24 3 7 9";
        assertEquals(result, expected, "operations test - numbers added in order");

        list.remove("7");
        list.removeFirst();
        list.remove("17");
        list.removeLast();
        list.remove("14");
        list.removeLast();

        result = list.toString();
        expected = "13 16 23 24";
        assertEquals(result, expected, "operations test - removed correct elements");
    }

    public static void testTraversal() {
        OrderedListADT<Integer> list = basicIntegerList();

        //RA: These are _extremely_ simple tests! You will need to write more!
        StringBuilder result = new StringBuilder();

        for (Integer i : list) {
            result.append(i).append(" ");
        }

        String expected = "1 3 7 9 13 14 16 17 23 24 ";
        assertEquals(result.toString(), expected, "traversal test - numbers added in order");
    }

    private static OrderedListADT<Integer> basicIntegerList() {
        OrderedListADT<Integer> list = new CompletedOrderedList<>();
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
        return list;
    }

    public static void testTraversalEmptyList() {
        OrderedListADT<Integer> list = new CompletedOrderedList<>();

        //RA: These are _extremely_ simple tests! You will need to write more!
        StringBuilder result = new StringBuilder();

        for (Integer i : list) {
            result.append(i).append(" ");
        }

        String expected = "";
        assertEquals(result.toString(), expected, "traversal test - empty list traversed");
    }

    public static void testTraversalWithNulls() {
        OrderedListADT<Integer> list = basicIntegerList();

        list.add(null);

        //RA: These are _extremely_ simple tests! You will need to write more!
        StringBuilder result = new StringBuilder();

        for (Integer i : list) {
            result.append(i).append(" ");
        }

        String expected = "1 3 7 9 13 14 16 17 23 24 ";
        assertEquals(result.toString(), expected, "traversal test - numbers added in order");
    }

    public static void main(String [] args) {
        // test deleting, inserting
        testIntegerList();
        testStringList();
        testTraversal();
        testTraversalEmptyList();

        try {
            testTraversalWithNulls();
            System.out.println("❌ Test: NullPointerException not thrown when inserting null into list");
        } catch (NullPointerException e) {
            System.out.println("✅ Test: NullPointerException thrown when inserting null");
        }
    }
}