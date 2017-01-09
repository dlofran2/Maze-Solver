/***************** dsets.h *******************
 * Definition of the Disjoint Set class 
 *********************************************/

#ifndef __DSETS_H__
#define __DSETS_H__

#include <vector>
#include <iostream>

using std::vector;

class DisjointSets {
	public:
		// Helper function that checks which element is bigger
		bool isBigger(int a, int b);

		// Creates n unconnected root nodes at the end of the vector
		void addelements(int num);

		// Compresses paths and finds the element in the set
		int find(int elem);

		// Implemented as union-by-size
		void setunion(int a, int b);

	private:
		vector<int> set;
};

#endif // __DSETS_H__