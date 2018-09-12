#ifndef ALGO_BRANCHES_HPP_INCLUDED
#define ALGO_BRANCHES_HPP_INCLUDED

#include "GraphIO.hpp"
#include <vector>

using namespace std;

//code to find a branch in a graph mainly three vertices that are 2*d distance apart
//respectiely .
//This permits to find brances in laminar graphs

bool branches(graph & g, int d ,vector<int> & assign);


#endif // ALGO_BRANCHES_HPP
