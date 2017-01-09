/**************** maze.h ******************
 * Definition of the Maze class
 ******************************************/

#ifndef __MAZE_H__
#define __MAZE_H__

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <queue>

#include "png.h"
#include "rgbapixel.h"
#include "dsets.h"

using std::map;
using std::pair;

class SquareMaze {
	public:
		SquareMaze();

		~SquareMaze();

		// makes a new SquareMaze of given height and width
		void makeMaze(int width, int height);
		
		// Determines whether it is possible or not to travel in a given direction
		bool canTravel(int x, int y, int dir) const;

		// Sets whether or not the specified wall exists
		void setWall(int x, int y, int dir, bool exists);

		// Solves 'this' square maze
		vector<int> solveMaze();

		// Draws the maze without the solution
		PNG * drawMaze() const;

		// Modifies the PNG from drawMaze to show the solution vector and the exit
		PNG * drawMazeWithSolution();

	private:
		// Hold the dimensions of the current maze
		int mazeHeight, mazeWidth, mazeSize;
		
		// 2D Boolean array that holds rightWalls and bottomWalls
		bool ** rightWalls;
		bool ** bottomWalls;
	
		// Holds the maze information like unions and stuffs
		DisjointSets disjointMazeSet;
};

#endif  // __MAZE_H__