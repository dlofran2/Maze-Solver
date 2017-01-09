#include "dsets.h"

/************* isBigger ****************
 * Calculates which set is bigger
 *
 * Input: a - Index of first element to check
 *        b - Index of second element to check
 * Output: true - a is bigger
 * 	   false - b is bigger
 **************************************/
bool DisjointSets::isBigger(int a, int b) {
	return set[a] <= set[b];
}

/************** addelements ***************
 * Creates n unconnected root nodes at the end of the vector.
 *
 * Input: num - The number of nodes to create
 * Output: n/a
 * ***************************************/
void DisjointSets::addelements(int num) {
	int oldSize = set.size();
	for (int i = oldSize; i < (oldSize + num); i++) {
		set.push_back(-1);
	}
}

/******************* find *******************
 * Compresses the paths and finds the element
 *
 * Input: elem - The element being seached for
 * Output: elemIndex - The index of the root of the
 * 		       up-tree in which the elem resides
 *******************************************/
int DisjointSets::find(int elem) {
	if (set[elem] < 0) 
		return elem;
	else
		return set[elem] = find(set[elem]);
}

/***************** setunion *****************
 * Implemented as union-by-size (increases distance to root
 * for fewest nodes).
 *
 * Input: a - Index of the first element to union
 *        b - Index of the second element to union
 * Output: n/a
 *
 * Restrictions:
 * -Cannot assume that the arguments are roots of existing uptrees
 * -Finds the roots of its arguments before combining the trees.
 * -If two sets are the the same size, the tee containing the second
 * argument will point to the tree containing the first.
 *******************************************/
void DisjointSets::setunion(int a, int b) {
	int newSize;

	a = find(a);
	b = find(b);
	newSize = set[a] + set[b];
	
	if (isBigger(a, b)) {
		set[b] = a;
		set[a] = newSize;
	} else {
		set[a] = b;
		set[b] = newSize;
	}
}

