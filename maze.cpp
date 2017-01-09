#include "maze.h"

using namespace std;

void setColor(int _red, int _green, int _blue, int x, int y, PNG * pixel) {
	(*pixel)(x, y)->red = _red;
	(*pixel)(x, y)->green = _green;
	(*pixel)(x, y)->blue = _blue;
}

SquareMaze::SquareMaze() {
	mazeWidth = 0;
	mazeHeight = 0;
	mazeSize = 0;
}

SquareMaze::~SquareMaze() {
	for (int i = 0; i < mazeWidth; i++) {
		delete [] bottomWalls[i];
		delete [] rightWalls[i];
	}

	delete [] bottomWalls;
	delete [] rightWalls;
}
/******************* makeMaze *******************
 * Makes a new SquareMaze of the given height and width.
 * If this object already represents a maze, clear all existing data 
 * creating the new maze. Random walls will be deleted without 
 * creating a cycle, until there are no more to be deleted without
 * creating a cycle.  
 *
 * DO NOT DELETE WALLS ON THE PERIMETER OF THE GRID. (don't want anyone escaping so easy)
 *
 * Input: width - The width of the SquareMaze (number of cells)
 * 	  height - The height of the SquareMaze (number of cells)
 * Output: n/a
 ***********************************************/
void SquareMaze::makeMaze(int width, int height) {
	mazeWidth = width;
	mazeHeight = height;
	mazeSize = width * height;

	// Now I just need to be sure to add a deconstructor
	
	// Essentially 'clears' all existing data.. Really just overwrites old data with walls
	rightWalls = new bool * [width];
	bottomWalls = new bool * [width];

	for (int i = 0; i < width; i++)  {
		rightWalls[i] = new bool [height];
		bottomWalls[i] = new bool [height];
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			rightWalls[i][j] = true;
			bottomWalls[i][j] = true;
		}
	}

	// Setting up the maze by deleting walls
	disjointMazeSet.addelements(mazeSize);
	int idx = 0;

	srand(time(NULL));
	while (idx < mazeSize - 1) {
		int randomNumber = rand();
		int randomized = randomNumber % mazeSize;
		int i = randomized % mazeWidth;
		int j = randomized / mazeWidth;
		int random = rand() % 2;		
	
		// right wall
		if (random == 0 && i != mazeWidth - 1) {	
			if (disjointMazeSet.find(randomized) != disjointMazeSet.find(randomized + 1)) {	
				disjointMazeSet.setunion(randomized, randomized + 1);
				rightWalls[i][j] = false;
				idx++;
			}
		}
		// bottom wall
		else if (random == 1 && j != mazeHeight - 1) {
			if (disjointMazeSet.find(randomized) != disjointMazeSet.find(randomized + mazeWidth)) {
				disjointMazeSet.setunion(randomized, randomized + mazeWidth);
				bottomWalls[i][j] = false;
				idx++;
			}
		} 
	} 	
}

/******************* canTravel ******************
 * Used to determine whether it is possible or not to travel in
 * a given direction.  
 *
 * YOU CANNOT STEP OFF THE MAZE OR THROUGH A WALL (you're not a ghost)
 *
 * Input: x - x coord of current cell
 * 	  y - y coord of current cell
 * 	  dir - The desired direction to move from the current
 * 	  	dir = 0 (rightward (+1 to the x coord))
 * 	  	dir = 1 (downward (+1 to the y coord))
 * 	  	dir = 2 (leftward (-1 to the x coord))
 * 	  	dir = 3 (upward (-1 to the y coord))
 * Output: true - you can travel that direction
 * 	   false - you cannot travel that direction
 ************************************************/
bool SquareMaze::canTravel(int x, int y, int dir) const {
	if (dir == 0) {
		if (rightWalls[x][y] == true || x > mazeWidth) {
			return false;
		} else {
			return true;
		}
	}

	if (dir == 1) {
		if (bottomWalls[x][y] == true || y > mazeHeight) {
			return false;
		} else {
			return true;
		}
	}

	if (dir == 2) {
		if (x <= 0 || rightWalls[x - 1][y] == true) {
			return false;
		} else {
			return true;
		}
	}

	if (dir == 3) {
		if (y <= 0 || bottomWalls[x][y - 1] == true) {
			return false;
		} else {
			return true;
		}
	}

	return false;
}

/****************** setWall **********************
 * Sets whether or not the wall exists and runs in O(1)
 * Does not prevent cycles from occuring, only sets a wall to 
 * be present or not present.
 * 
 * Input: x - x coord of current cell
 *  	  y - y coord of current cell
 *  	  dir - direction of which wall to set
 *  	  	dir = 0 (rightward (+1 to the x coord)
 *  	  	dir = 1 (downward (+1 to the y coord)
 *  	  		Only two needed
 *  	  exists - true if setting the wall to exist, false otherwise
 * Output: n/a
 *************************************************/
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
	// Check for bottom wall
	if (dir) {
		bottomWalls[x][y] = exists;
	} 

	// Check for right wall
	else {
		rightWalls[x][y] = exists;
	}
}

/***************** solveMaze *********************
 * For each square on the bottom row, there is a distance from the origin.
 * This is defined as the length (# of step) of the only path through the maze
 * from the origin to that square.  
 * Select the square in the bottom row with the largest distance from the origin.
 * 
 * If multiple paths of max length exist, use the one with the destination
 * cell that has the smallest x value.
 *
 * Input: n/a
 * Output: directions - a vector of directions taken to solve the maze
 ************************************************/
vector<int> SquareMaze::solveMaze() {
	int backtrack = 0;
	int bottom = 0;
	int x = 0;
	int y = 0;
	int xSolution = 0;
	int ySolution = 0;

	vector<int> directions;
	vector<int> solution;
	vector<vector<bool>> visited(mazeWidth, vector<bool>(mazeHeight));

	for (int i = 0; i < mazeWidth; i++) {
		for (int j = 0; j < mazeHeight; j++) {
			visited[i][j] = false;
		}
	}

	visited[x][y] = true;
	
	for (int i = 0; i < ((mazeHeight * mazeWidth) * 2); i++) {
		backtrack = 0;
		if (canTravel(x, y, 0) && visited[x + 1][y] != true) {
			visited[x][y] = true;
			x++;
			directions.push_back(0);

			backtrack = 1;
		}

		else if (canTravel(x, y, 1) && visited[x][y + 1] != true) {
			visited[x][y] = true;
			y++;
			directions.push_back(1);
	
			backtrack = 1;
		}

		else if (canTravel(x, y, 2) && visited[x - 1][y] != true) {
			visited[x][y] = true;
			x--;
			directions.push_back(2);

			backtrack = 1;
		}

		else if (canTravel(x, y, 3) && visited[x][y - 1] != true) {
			visited[x][y] = true;
			y--;
			directions.push_back(3);
			
			backtrack = 1;
		}

		if (y == mazeHeight - 1) {
			if (solution.size() < directions.size()) {
				solution = directions;
			}
			if (visited[x][y] != true) {
				bottom++;
					if (bottom == (mazeWidth - 1)) 
						break;
			}
		}
			
		if (backtrack == 0) {
			int btValue = directions.back();

			directions.pop_back();
			visited[x][y] = true;

			if (btValue == 0) 
				x--;
			else if (btValue == 1)
				y--;
			else if (btValue == 2)
				x++;
			else if (btValue == 3)
				y++;
		}
	}
		
	
	return solution;
}

/****************** drawMaze ********************
 * Draws the maze w/o the solution. 
 *
 * Steps:
 * 1. Create a new PNG and set the dimensions to (width * 10 + 1, height * 10 + 1).
 * 2. Blacken the entire topmost row and leftmost column of pixels except (1,0) and (9,0)
 * 3. If a right wall exists, blacken the pixels with coordinates ((x + 1) * 10, (y * 10) + k) for k from 0 to 10.
 * 4. If a bottom wall exists, blacken the pixels with coordinates (x * 10) + k, (y + 1) * 10) for k from 0 to 10.
 *
 * Input: n/a
 * Output: unsolvedMaze - a PNG of the unsolved SquareMaze
 ************************************************/
PNG * SquareMaze::drawMaze() const {
	// 1. Create a new PNG and set the dimensions
	int PNGWidth = mazeWidth * 10 + 1;
	int PNGHeight = mazeHeight * 10 + 1;
	PNG * unsolvedMaze = new PNG(PNGWidth, PNGHeight);
	
	// 2.  Black the topmost row and leftmost column of pixels
	for (int i = 10; i < PNGWidth; i++) {
		setColor(0, 0, 0, i, 0, unsolvedMaze);
	}

	for (int i = 0; i < PNGHeight; i++) {
		setColor(0, 0, 0, 0, i, unsolvedMaze);
	}

	// Iterate through the cells now determining if a wall exists
	for (int i = 0; i < mazeWidth; i++) {
		for (int j = 0; j < mazeHeight; j++) {
			// 3. If a right wall exists, black the coordinates
			if (rightWalls[i][j]) {
				for (int k = 0; k < 11; k++) {
					setColor(0, 0, 0, (i + 1) * 10, j * 10 + k, unsolvedMaze);
				}
			}
			if (bottomWalls[i][j]) {
				for (int k = 0; k < 11; k++) {
					setColor(0, 0, 0, i * 10 + k, (j + 1) * 10, unsolvedMaze);
				}
			}
		}
	}
	
	return unsolvedMaze;
}

/************* drawMazeWithSolution ************
 * 1. Start at (5, 5).
 * 2. Each direction in the solution vector corresponds to a trail of 11 red pixels
 * 3. Make the exit by undoing the bottom wall of the destination square.
 *    Call the destination maze coordinates and whiten the pixels (formula in Doxygen)
 * Input: n/a
 * Output: solvedMaze -  A PNG of the solved SquareMaze
 ***********************************************/
PNG * SquareMaze::drawMazeWithSolution() {
	PNG * solvedMaze = drawMaze();
	vector<int> path = solveMaze();
	int x = 5, y = 5;

	for (size_t i = 0; i < path.size(); i++) {
		// right
		if (path[i] == 0) {
			for (int j = 0; j < 11; j++) {
				setColor(255, 0, 0, x, y, solvedMaze);
				x++;
			}
			x--;
		}
		
		// down
		else if (path[i] == 1) {
			for (int j = 0; j < 11; j++) {
				setColor(255, 0, 0, x, y, solvedMaze);
				y++;
			}
			y--;
		}

		// left
		else if (path[i] == 2) {
			for (int j = 0; j < 11; j++) {
				setColor(255, 0, 0, x, y, solvedMaze);
				x--;
			}
			x++;
		}

		// up
		else if (path[i] == 3) {
			for (int j = 0; j < 11; j++) {
				setColor(255, 0, 0, x, y, solvedMaze);
				y--;
			}
			y++;
		}
	}
	
	// Draw exit
	x -= 4;
	y += 5;
	for (int i = 0; i < 9; i++) {
		setColor(255, 255, 255, x, y, solvedMaze);
		x++;
	}
	
	return solvedMaze;
}

