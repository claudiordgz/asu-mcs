package com.RodriguezRodriguezMazeGen;

import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Random;

public class RodriguezRodriguezMazeGen {

    private static char[][] createBaseLevel(int width, int height) {
        char[][] frame = new char[height][width];
        char blank_string = ' ';
        char wall_string = '#';
        for (int i = 0; i != height; ++i) {
            for (int j = 0; j != width; ++j) {
                if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    frame[i][j] = wall_string;
                } else {
                    frame[i][j] = blank_string;
                }
            }
        }

        return frame;
    }

    private static int randBetween(int min, int max) {
        return (int) (Math.random() * ((max - min))) + min;
    }

    private static void makeMazeRecursive(char[][] level, int startX, int startY, int endX, int endY) {

        // base cases
        if ((endX - startX) <=2 )  {
            return;
        }
        if ((endY - startY) <=2)  {
            return;
        }

        // random new X and new y coordinates
        int newY = randBetween(startY+1, endY-1);
        int newX  = randBetween(startX+1, endX-1);

        //create new column
        for (int i = startY; i <= endY; i++) {
            level[i][newX] = '#';
        }
        level[randBetween(startY, newY-1)][newX] = ' '; //random hole from startY to newY with newX(upper)
        level[randBetween(newY-1, endY)][newX] = ' '; //random hole from newY to endY with newX(lower)
        level[randBetween(startY, endY)][newX] = ' '; //random hole from startY to endY with newX (anywhere from top to bottom)

        //create new row
        for (int j = startX; j <= endX; j++) {
            level[newY][j] = '#';
        }
        level[newY][randBetween(startX, newX-1)] = ' '; // random hole from startX to new X (Left)
        level[newY][randBetween(newX, endX)] = ' '; // random hole from newX to endX (Right)
        level[newY][randBetween(startX, endX)] = ' '; // random hole from startX to endX with newY (anywhere from left to right)


        // call each sub-areas using recursion(total 4 areas)
        makeMazeRecursive(level, startX+1, startY+1, newX-1, newY-1);  //upper left part
        makeMazeRecursive(level, newX+1, startY+1 , endX-1  , newY-1); //upper right part
        makeMazeRecursive(level, startX+1, newY+1, newX-1, endY-1); //lower left part
        makeMazeRecursive(level, newX+1, newY+1, endX-1, endY-1); //lower right part

    }

    public static void displayMaze(char[][] maze, int width, int height) {
        for (int y = 0; y != height; ++y)
        {
            for (int x = 0; x != width; ++x) {
                System.out.print(maze[y][x]);
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        int width = 100;
        int height = 30;
        char[][] maze = createBaseLevel(width, height);
        makeMazeRecursive(maze,1,1,width - 1, height - 1);

        displayMaze(maze, width, height);
    }
}
