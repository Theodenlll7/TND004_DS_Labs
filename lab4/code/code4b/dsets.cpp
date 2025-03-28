/*********************************************
 * file:	~\code4b\dsets.cpp                *
 * remark: implementation of disjoint sets    *
 **********************************************/

#include <iostream>
#include <format>
#include <cassert>

#include "dsets.h"

 // -- CONSTRUCTORS

DSets::DSets(size_t theSize) : V(theSize + 1) {  // slot zero is not used
	assert(theSize > 0);
	init();
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
	for (size_t i = 1; i < V.size(); ++i) {
		V[i] = -1;  // every disjoint set has one node -- singleton
	}
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
	assert(r != s);
	assert(r >= 1 && r <= V.size() - 1);
	assert(s >= 1 && s <= V.size() - 1);
	assert(V[r] < 0);
	assert(V[s] < 0);

	// simple union
	//V[r] = s;

	// *** TODO ***
	// Do Union by Size
	if (V[s] < V[r]) { //Check for which one has a bigger size (indecatade by negative numbers)
		V[s] += V[r]; //Increse the size count the size count of V[R]
		V[r] = s; //Make the root of the r set a node in the s set.
	}
	else {
		V[s] = r; //Make the root of the s set a node in the r set.
		V[r] += V[s];//Increse the size count the size count of V[R]
	}
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
	assert(x >= 1 && x <= V.size() - 1);

	// simple find
   /* if (V[x] < 0) {
		return x;
	} else {
		return find(V[x]);
	}*/

	// *** TODO ***
	// find with path compression


	if(V[x] < 0) { //Base case x is the root
		return x;
	}
	else {
		//Make vertecis pathed through, point to root
		V[x] = find(V[x]); //Connect all direcly to root
		return V[x];
	}
}

// just in case ...
void DSets::print() const {
	std::cout << "\n";

	for (size_t i = 1; i < V.size(); i++) {
		std::cout << std::format("{:4}", i);
	}
	std::cout << "\n";
	for (size_t i = 1; i < V.size(); i++) {
		std::cout << std::format("{:4}", V[i]);
	}
	std::cout << "\n";
}
