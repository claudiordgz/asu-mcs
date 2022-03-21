package edu.ser222.m01_02;

/**
 * An implementation of the Matrix ADT. Provides four basic operations over an immutable type.
 * 
 * Last updated 7/31/2021.
 * 
 * @author (your name), Ruben Acuna
 * @version (version)
 */

public class CompletedMatrix implements Matrix {

    public CompletedMatrix(int[][] matrix) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public int getElement(int y, int x) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public int getRows() {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public int getColumns() {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public Matrix scale(int scalar) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public Matrix plus(Matrix other) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public Matrix minus(Matrix other) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    @Override
    public Matrix multiply(Matrix other) {
        throw new java.lang.UnsupportedOperationException("TODO!");
    }

    /**
     * Entry point for matrix testing.
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        //These tests show sample usage of the matrix, and some basic ideas for testing. They are not comprehensive.

        int[][] data1 = new int[0][0];
        int[][] data2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        int[][] data3 = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
        int[][] data4 = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
        int[][] data5 = {{1, 4, 7}, {2, 5, 8}};

        Matrix m1 = new CompletedMatrix(data1);
        Matrix m2 = new CompletedMatrix(data2);
        Matrix m3 = new CompletedMatrix(data3);
        Matrix m4 = new CompletedMatrix(data4);
        Matrix m5 = new CompletedMatrix(data5);

        System.out.println("m1 --> Rows: " + m1.getRows() + " Columns: " + m1.getColumns());
        System.out.println("m2 --> Rows: " + m2.getRows() + " Columns: " + m2.getColumns());
        System.out.println("m3 --> Rows: " + m3.getRows() + " Columns: " + m3.getColumns());

        //check for reference issues
        System.out.println("m2 -->\n" + m2);
        data2[1][1] = 101;
        System.out.println("m2 -->\n" + m2);

        //test equals
        System.out.println("m2==null: " + m2.equals(null));             //false
        System.out.println("m3==\"MATRIX\": " + m2.equals("MATRIX"));   //false
        System.out.println("m2==m1: " + m2.equals(m1));                 //false
        System.out.println("m2==m2: " + m2.equals(m2));                 //true
        System.out.println("m2==m3: " + m2.equals(m3));                 //false
        System.out.println("m3==m4: " + m3.equals(m4));                 //true

        //test operations (valid)
        System.out.println("m1 + m1:\n" + m1.plus(m1));
        System.out.println("m1 + m1:\n" + m1.plus(m1));
        System.out.println("2 * m2:\n" + m2.scale(2));
        System.out.println("m2 + m3:\n" + m2.plus(m3));
        System.out.println("m2 - m3:\n" + m2.minus(m3));
        System.out.println("3 * m5:\n" + m5.scale(3));

        //not tested... multiply(). you know what to do.

        //test operations (invalid)
        //System.out.println("m1 + m2" + m1.plus(m2));
        //System.out.println("m1 + m5" + m1.plus(m5));
        //System.out.println("m1 - m2" + m1.minus(m2));
    }
}