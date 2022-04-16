package edu.ser222.m02_01;


import java.text.DecimalFormat;
import java.util.Random;

/**
 * (basic description of the program or class)
 *
 * Completion time: 4hrs
 *
 * @author Claudio Rodriguez, Acuna, Sedgewick
 * @version 04/09/2022
 */

public class CompletedBenchmarkTool implements BenchmarkTool {

    /***************************************************************************
     * START - SORTING UTILITIES, DO NOT MODIFY (FROM SEDGEWICK)               *
     **************************************************************************/

    public static void insertionSort(Comparable[] a) {
        int N = a.length;

        for (int i = 1; i < N; i++)
        {
            // Insert a[i] among a[i-1], a[i-2], a[i-3]... ..
            for (int j = i; j > 0 && less(a[j], a[j-1]); j--)
                exch(a, j, j-1);
        }
    }


    public static void shellsort(Comparable[] a) {
        int N = a.length;
        int h = 1;

        while (h < N/3) h = 3*h + 1; // 1, 4, 13, 40, 121, 364, 1093, ...

        while (h >= 1) {
            // h-sort the array.
            for (int i = h; i < N; i++) {
                // Insert a[i] among a[i-h], a[i-2*h], a[i-3*h]... .
                for (int j = i; j >= h && less(a[j], a[j-h]); j -= h)
                exch(a, j, j-h);
            }
            h = h/3;
        }
    }


    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }


    private static void exch(Comparable[] a, int i, int j) {
        Comparable t = a[i]; a[i] = a[j]; a[j] = t;
    }

    /***************************************************************************
     * END - SORTING UTILITIES, DO NOT MODIFY                                  *
     **************************************************************************/

    //TODO: implement interface methods.

    public static void main(String args[]) {
        BenchmarkTool me = new CompletedBenchmarkTool();
        int size = 40960;

        //NOTE: feel free to change size here. all other code must go in the
        //      methods.

        me.runBenchmarks(size);
    }

    @Override
    public Integer[] generateTestDataBinary(int size) {
        int half = size / 2;
        Integer[] testData = new Integer[size];
        for (int i = 0; i != half; i++) {
            testData[i] = 0;
        }
        for (int i = half; i != size; i++) {
            testData[i] = 1;
        }
        return testData;
    }

    @Override
    public Integer[] generateTestDataHalves(int size) {
        Integer [] testData = new Integer[size];
        int count = 0;
        int half = size/2;
        for(int i =0; i != size; i++){
            if (i >= half) {
                half = half + (size - i) / 2;
                count++;
            }
            testData[i] = count;
        }
        return testData;
    }

    @Override
    public Integer[] generateTestDataHalfRandom(int size) {
        Integer [] testData = new Integer[size];
        Random random = new Random();
        int half = size/2;
        for (int i = 0; i != half; i++) {
            testData[i] = 0;
        }
        for (int i = half; i != size; i++) {
            testData[i] = random.nextInt(Integer.MAX_VALUE);
        }
        return testData;
    }

    @Override
    public double computeDoublingFormula(double t1, double t2) {
        return Math.log(t2/t1) / Math.log(2);
    }

    @Override
    public double benchmarkInsertionSort(Integer[] small, Integer[] large) {
        Stopwatch sw1 = new Stopwatch();
        insertionSort(small);
        double t1Time = sw1.elapsedTime();

        Stopwatch sw2 = new Stopwatch();
        insertionSort(large);
        double t2Time = sw2.elapsedTime();

        System.out.println("t1Time: " + t1Time);
        System.out.println("t2Time: " + t2Time);
        
        return computeDoublingFormula(t1Time, t2Time);
    }

    @Override
    public double benchmarkShellsort(Integer[] small, Integer[] large) {
        Stopwatch sw1 = new Stopwatch();
        shellsort(small);
        double t1Time = sw1.elapsedTime();

        Stopwatch sw2 = new Stopwatch();
        shellsort(large);
        double t2Time = sw2.elapsedTime();

        System.out.println("t1Time: " + t1Time);
        System.out.println("t2Time: " + t2Time);
        
        return computeDoublingFormula(t1Time, t2Time);
    }

    @Override
    public void runBenchmarks(int size) {
        Integer[] smBinary = generateTestDataBinary(size);
        Integer[] smHalves = generateTestDataHalves(size);
        Integer[] smRandom = generateTestDataHalfRandom(size);
        
        Integer[] lgBinary = generateTestDataBinary(size * 2);
        Integer[] lgHalves = generateTestDataHalves(size * 2);
        Integer[] lgRandom = generateTestDataHalfRandom(size * 2);

        double insertionSortBin = benchmarkInsertionSort(smBinary, lgBinary);
        double insertionSortHalf = benchmarkInsertionSort(smHalves, lgHalves);
        double insertionSortRanInt = benchmarkInsertionSort(smRandom, lgRandom);

        double shellSortBin = benchmarkShellsort(smBinary, lgBinary);
        double shellSortHalf = benchmarkShellsort(smHalves, lgHalves);
        double shellSortRanInt = benchmarkShellsort(smRandom, lgRandom);

        DecimalFormat output = new DecimalFormat("0.000");
        System.out.println("\t\t\tInsertion\tShellsort");
        System.out.println("Bin\t\t\t" + output.format(insertionSortBin)
                + "\t\t" + output.format(shellSortBin));
        System.out.println("Half\t\t" + output.format(insertionSortHalf)
                + "\t\t" + output.format(shellSortHalf));
        System.out.println("RanInt\t\t" + output.format(insertionSortRanInt)
                + "\t\t" + output.format(shellSortRanInt));
    }
}