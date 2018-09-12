#ifndef ALGO_HPP_INCLUDED
#define ALGO_HPP_INCLUDED

#include <vector>
#include <utility>
#include <list>

#include "GraphIO.hpp"

using namespace std;

//TODO clean up

/* An edge class that permits to do certain community 
 * fusion algorithms to run faster in connection with
 * linked lists
 *

template <typename T> class edge{
	T data;	
	typename list<edge<T>>::iterator rev;

	public:
	edge(T);
	T getData();
	void updateIt(typename list<edge<T>>::iterator it);
	typename list<edge<T>>::iterator getIt();
}; */

struct cliqueTree{
	int n; //original size of the graph
	vector<vector<int> > cliques;
	//edges contain parent and seperator
	vector<pair<int,vector<int> > > edges;
};

void reorderNeighbourhoood(vector<vector<int> > & , 
								vector<vector<int > > & , vector<int> & );

void lexBFS(graph & g, vector<int> & per, vector<int> & inv);

void lexBFS_plus(graph & g, vector<int> & per, 
								vector<int> & inv, vector<int> & init);

void multiSweep(int k, graph & g, vector<int> & per, vector<int> & inv);

void calcCT(graph & g, cliqueTree & ct);

#endif // ALGO_HPP_INCLUDE

