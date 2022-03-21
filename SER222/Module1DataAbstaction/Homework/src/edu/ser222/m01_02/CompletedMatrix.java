package edu.ser222.m01_02;

/**
 * An implementation of the Matrix ADT. Provides four basic operations over an immutable type.
 * 
 * Last updated 7/31/2021.
 * 
 * @author Claudio Rodriguez Rodriguez, Ruben Acuna
 * @version 03/19/2022
 */

public class CompletedMatrix implements Matrix {

    private final int[][] _matrix;
    private final int rows;
    private final int cols;

    public CompletedMatrix(final int[][] matrix) {
        if (matrix == null) {
            throw new IllegalArgumentException("Matrix arg can't be null");
        }
        rows = matrix.length;
        if (rows == 0) {
            cols = 0;
            
            throw new IllegalArgumentException("Matrix arg can't be null");
        }

        cols = matrix[0].length;

        _matrix = matrix; // new int[matrix.length][matrix[0].length];
        for (int i = 0; i != matrix.length; ++i) {
            for (int j = 0; j != matrix[0].length; ++j) {
                _matrix[i][j] = matrix[i][j];
            }
        }
    }

    @Override
    public int getElement(int y, int x) {
        return _matrix[y][x];
    }

    @Override
    public int getRows() {
        return rows;
    }

    @Override
    public int getColumns() {
        return cols;
    }

    @Override
    public Matrix scale(int scalar) {
        int[][] newMatrix = new int[rows][cols];

        for (int m = 0; m != newMatrix.length; ++m) {
            for (int n = 0; n != newMatrix[0].length; ++n) {
                newMatrix[m][n] = _matrix[m][n] * scalar;
            }
        }

        return new CompletedMatrix(newMatrix);
    }

    @Override
    public Matrix plus(Matrix other) {
        int[][] result = validateMatrixAndGetResult(other);
        for (int i = 0; i != rows; i++) {
            for (int j = 0; j != cols; j++) {
                result[i][j] = this.getElement(i, j) + other.getElement(i, j);
            }
        }
        return new CompletedMatrix(result);
    }

    @Override
    public Matrix minus(Matrix other) {
        int[][] result = validateMatrixAndGetResult(other);
        for (int i = 0; i != rows; i++) {
            for (int j = 0; j != cols; j++) {
                result[i][j] = this.getElement(i, j) - other.getElement(i, j);
            }
        }
        return new CompletedMatrix(result);
    }

    private int[][] validateMatrixAndGetResult(Matrix other) {
        if (other == null)
            throw new IllegalArgumentException("Invalid input matrix");

        if (rows == 0 || other.getRows() == 0)
            throw new RuntimeException("Matrices can't be empty");

        if (this.isDifferentSize(other))
            throw new RuntimeException("Matrices must be the same size");

        return new int[rows][cols];
    }

    private int multiplyCell(Matrix other, int row, int col) {
        int cell = 0;
        for (int i = 0; i < this.getColumns(); i++) {
            cell += this.getElement(row, i) * other.getElement(i, col);
        }
        return cell;
    }

    @Override
    public Matrix multiply(Matrix other) {
        if (other == null)
            throw new IllegalArgumentException("Invalid input matrix");

        if (rows == 0 || other.getRows() == 0)
            return new CompletedMatrix(this._matrix);

        if (this.getColumns() != other.getRows())
            throw new IllegalArgumentException("Matrix 1 cols must match Matrix 2 rows");

        int r1 = this.getRows();
        int c2 = other.getColumns();
        int[][] result = new int[r1][c2];
        for (int i = 0; i != r1; ++i) {
            for (int j = 0; j != c2; ++j) {
                result[i][j] = multiplyCell(other, i, j);
            }
        }

        return new CompletedMatrix(result);
    }

    private boolean isDifferentSize(Matrix other) {
        return (rows != other.getRows() || cols != other.getColumns());
    }

    @Override
    public boolean equals(Object other) {
        if (!(other instanceof CompletedMatrix)) return false;

        if (other.hashCode() == this.hashCode()) return true;

        Matrix otherM = (Matrix) other;
        if (rows == 0 && otherM.getRows() == 0) return true;

        if (cols == 0 && otherM.getColumns() == 0) return true;

        if (this.isDifferentSize(otherM)) return false;

        return _isEqual(otherM);
    }

    private boolean _isEqual(Matrix otherM) {
        for (int y = 0; y != rows; ++y)
            for (int x = 0; x != cols; ++x)
                if (this.getElement(y, x) != otherM.getElement(y, x))
                    return false;
        return true;
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();

        for (int[] row : _matrix) {
            for (int cell : row) {
                output.append(cell).append(" ");
            }
            output.append("\n");
        }

        return output.toString();
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
        //System.out.println("m1 + m1:\n" + m1.plus(m1));
        //System.out.println("m1 + m1:\n" + m1.plus(m1));
        System.out.println("2 * m2:\n" + m2.scale(2));
        System.out.println("m2 + m3:\n" + m2.plus(m3));
        System.out.println("m2 - m3:\n" + m2.minus(m3));
        System.out.println("3 * m5:\n" + m5.scale(3));

        //not tested... multiply(). you know what to do.
        //System.out.println("m1 * m1:\n" + m1.multiply(m1)); // invalid
        //System.out.println("m1 * m2:\n" + m1.multiply(m2)); // invalid
        System.out.println("m2 * m2:\n" + m2.multiply(m2));
        System.out.println("m2 * m3:\n" + m2.multiply(m3));
        //System.out.println("m4 * m5:\n" + m4.multiply(m5)); // invalid
        System.out.println("m5 * m4:\n" + m5.multiply(m4));

        //test operations (invalid)
        //System.out.println("m1 + m2" + m1.plus(m2));
        //System.out.println("m1 + m5" + m1.plus(m5));
        //System.out.println("m1 - m2" + m1.minus(m2));
    }
}