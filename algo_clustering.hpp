#ifndef ALGO_CLUSTERING_HPP_INCLUDED
#define ALGO_CLUSTERING_HPP_INCLUDED

#include "GraphIO.hpp"
#include <vector>

/* TODO 
 * remove lazy deletion
 * dangerous on large graphs
 */
void lexDFS(graph & g, int start, vector<int> &per, vector<int> &inv);

void compactCommunities(graph & g, edges & e, int l, int&, vector<int> &);

void compactCommunities2(graph & g, graph_edge_nb & g1, edges & e, int l, int & nbCommunities, vector<int>& assign);

void newman_greedy_clustering(graph & g, edges & e, int & , vector<int>& );

#endif
