#ifndef PARTITION_REFINEMENT_INTERVAL_HPP_INCLUDED
#define PARTITION_REFINEMENT_INTERVAL_HPP_INCLUDED


#include <vector>
#include <stack>

#include "Elem.hpp"
#include "Klass.hpp"

using namespace std;

class PartRefInter{
	int n,nbCliques;
	int timer,counter;
	stack<int> pivots;
	//vector<int> per,inv;
	vector<bool> used_pivots,used_edges;
	vector<int> edge_count;

	vector<vector<int > > & vertices_in_clique;	
	vector<vector<int > > & edges;	
	vector<vector<int > > & seperators;	
	
	//debug
	vector<vector<int > > & cliques;

	vector<Elem*> elements;
	Klass * begin, * end, *current;
	Elem * first, * last;

 	void incrementEdgesCount(vector<int> & edges);

	public:

	PartRefInter(int n, vector<vector<int> > & v_in_c, vector<vector<int> > & edges,
									vector<vector<int > > & seperators,vector<vector<int> > & cliques);
	~PartRefInter();
	void partition(vector<int> & clique_order);
};

#endif // PARTITION_REFINEMENT_HPP_INCLUDED 
