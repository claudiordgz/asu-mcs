package edu.ser222.m02_02;

/**
 * Implements various divide and conquer algorithms.
 *
 * Last updated 4/2/2022.
 *
 * Completion time: 4hrs
 *
 * @author  Claudio Rodriguez, Acuna, Sedgewick and Wayne
 * @version 04/09/2022
 */
import java.util.Random;

public class CompletedMerging implements MergingAlgorithms {

    @Override
    public <T extends Comparable> Queue<T> mergeQueues(Queue<T> q1, Queue<T> q2) {
        Queue<T> q3 = new ListQueue<T>();
        while (!q1.isEmpty() && !q2.isEmpty()) {
            if (less(q1.peek(), q2.peek())) {
                q3.enqueue(q1.dequeue());
            } else {
                q3.enqueue(q2.dequeue());
            }
        }
        while (!q1.isEmpty()) {
            q3.enqueue(q1.dequeue());
        }
        while (!q2.isEmpty()) {
            q3.enqueue(q2.dequeue());
        }
        return q3;
    }

    @Override
    public void sort(Comparable[] a) {
        Comparable[] merged = new Comparable[a.length];
        merged = mergesort(a);
        for (int i = 0; i != a.length; i++) {
            a[i] = merged[i];
        }
    }

    @Override
    public Comparable[] mergesort(Comparable[] a) {
        if (a.length <= 1) {
            return a;
        }

        int midpoint = a.length / 2;
        Comparable[] left = new Comparable[midpoint];
        Comparable[] right = new Comparable[a.length - midpoint];

        for (int i = 0; i < midpoint; i++) {
            left[i] = a[i];
        }
        int j = 0;
        for (int i = midpoint; i < a.length; i++) {
            right[j] = a[i];
            j++;
        }

        left = mergesort(left);
        right = mergesort(right);

        return merge(left, right);
    }

    @Override
    public Comparable[] merge(Comparable[] a, Comparable[] b) {
        Comparable[] merged = new Comparable[a.length + b.length];
        int left_index = 0;
        int right_index = 0;
        int current_index = 0;

        while (left_index < a.length && right_index < b.length) {
            if (a[left_index].compareTo(b[right_index]) < 0) {
                merged[current_index] = a[left_index];
                left_index += 1;
            } else {
                merged[current_index] = b[right_index];
                right_index += 1;
            }
            current_index += 1;
        }

        while(left_index < a.length)
    	{
    		merged[current_index] = a[left_index];
    		left_index++;
    		current_index++;
    	}
    	while(right_index < b.length)
    	{
    		merged[current_index] = b[right_index];
    		right_index++;
    		current_index++;
    	}

        return merged;
    }

    public static <T> ListQueue<T> _shuffle(ListQueue<T> list) {
        if (list.size() <= 1) {
            return list;
        }

        ListQueue<T> left = new ListQueue<T>();
        ListQueue<T> right = new ListQueue<T>();
        while (!list.isEmpty()) {
            left.enqueue(list.dequeue());
            if (!list.isEmpty()) {
                right.enqueue(list.dequeue());
            }
        }
        left = _shuffle(left);
        right = _shuffle(right);

        return _shuffleMerge(left, right);
    }

    public static <T> ListQueue<T> _shuffleMerge(ListQueue<T> a, ListQueue<T> b) {
        ListQueue<T> shuffled = new ListQueue<T>();
        Random random = new Random();
        while (a.size() != 0 && b.size() != 0) {
            if (random.nextBoolean()) {
                shuffled.enqueue(a.dequeue());
            } else {
                shuffled.enqueue(b.dequeue());
            }
        }

        while(!a.isEmpty())
        {
            shuffled.enqueue(a.dequeue());
        }
        while(!b.isEmpty())
        {
            shuffled.enqueue(b.dequeue());
        }
        return shuffled;
    }

    @Override
    public void shuffle(Object[] a) {
        if (a.length <= 1) {
            return;
        }
        ListQueue list = new ListQueue();
        for (int i = 0; i != a.length; i++) {
            list.enqueue(a[i]);
        }
        ListQueue shuffled = _shuffle(list);
        for (int i = a.length - 1; i >= 0; i--) {
            a[i] = shuffled.dequeue();
        }
    }

    /**
     * entry point for sample output.
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Queue<String> q1 = new ListQueue<>(); q1.enqueue("E"); q1.enqueue("L"); q1.enqueue("O"); q1.enqueue("R"); q1.enqueue("T");
        Queue<String> q2 = new ListQueue<>(); q2.enqueue("A"); q2.enqueue("E"); q2.enqueue("M"); q2.enqueue("P"); q2.enqueue("S"); q2.enqueue("X");
        Queue<Integer> q3 = new ListQueue<>(); q3.enqueue(5); q3.enqueue(12); q3.enqueue(15); q3.enqueue(17); q3.enqueue(20);
        Queue<Integer> q4 = new ListQueue<>(); q4.enqueue(1); q4.enqueue(4); q4.enqueue(12); q4.enqueue(13); q4.enqueue(16); q4.enqueue(18);

        MergingAlgorithms ma = new CompletedMerging();

        //Q1 - sample test cases
        Queue merged1 = ma.mergeQueues(q1, q2);
        System.out.println(merged1.toString());
        Queue merged2 = ma.mergeQueues(q3, q4);
        System.out.println(merged2.toString());

        // Queue<Integer> q5 = new ListQueue<>();
        // Queue<Integer> q6 = new ListQueue<>();
        // Queue merged3 = ma.mergeQueues(q5, q6);
        // System.out.println(merged3.toString());

        //Q2 - sample test cases
        String[] a = {"S", "O", "R", "T", "E", "X", "A", "M", "P", "L", "E"};
        ma.sort(a);
        assert isSorted(a);
        show(a);

        //Q3 - sample test cases
        String[] b = {"S", "O", "R", "T", "E", "X", "A", "M", "P", "L", "E"};
        ma.shuffle(b);
        show(b);

        ma.shuffle(b);
        show(b);
    }

    //below are utilities functions, please do not change them.

    //sorting helper from text
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    //sorting helper from text
    private static void show(Comparable[] a) {
        for (Comparable a1 : a)
            System.out.print(a1 + " ");

        System.out.println();
    }

    //sorting helper from text
    public static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i-1]))
                return false;

        return true;
    }
}