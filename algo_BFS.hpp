#ifndef ALGO_BFS_HPP_INCLUDED
#define ALGO_BFS_HPP_INCLUDED

#include"GraphIO.hpp"
#include <queue>
#include <stack>
#include <vector>

using namespace std;


// normal BFS return depth (aka excentricity of start)
int BFS(graph & g, int start, vector<int> & par , int & last);

// normal BFS returns neighbours depth away
void BFS_depth(graph & g, int start, int depth, vector<int> & res);

// calculates all distances 
void BFS_ALL_DIS(graph & g, int ** dis);

//same as BFS
//but pushes from first to last discovered node with distance into s
int BFS_tree(graph & g, int start, vector<int> & par, stack<pair<int,int> > & s);

//Create the BFS tree of g starting at start.
BFSTree BFS_tree_full(graph & g, int start);

//same as BFS but stops if depth > k (return k+1 in this case)
int BFS_short_circuited(graph & g, int start, bool * used, int k);

//returns the distance from start to the diametral path
//-1 if not connected
int BFS_diametral_path(graph & g, int start, bool * used , const vector<bool> &);//bool * const diametral_path);

//does tie-break by the order in per 
void BFS_plus(graph & g, vector<int> & per, vector<int> & res);

#endif
