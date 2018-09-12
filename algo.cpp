#include <algorithm>

#include "algo.hpp"
#include "algo_BFS.hpp"
#include "partition_refinement.hpp"

//TODO only debug /clean up
#include <iostream>
#include <cassert>
#include "test.hpp"

/*template <typename T> class edge::edge(T data) : data(data){}

T template <typename T> class edge::getData(T data) : data(data){
	return this->data;
}

template <typename T> class edge::updateIt(typename list<edge<T>>::iterator it){
	this->rev = it;
}

typename list<edge<T>>::iterator template <typename T> class edge::getIt(){
	return this->rev;
} */

void lexBFS(graph & g, vector<int> & per,vector<int> & inv){

	int n = g.n; 
	vector<int> init(n,0);
	for(int i=0;i<n;++i){
		init[i] = i;
	} 
	//vector<int> init = {1,4,3,6,7,2,5,0};

	PartRef pr(init);
	pr.partition(g.g);

	per = pr.per;
	inv = pr.inv;
}

void reorderNeighbourhoood(vector<vector<int> > & orig, 
								vector<vector<int > > & new_order, vector<int> & order){

	for(vector<int>::iterator it = order.begin(); it != order.end(); ++it){
		//assert(0 <= *it && (size_t)*it < orig.size());
		for(vector<int>::iterator it2 = orig[*it].begin(); it2 != orig[*it].end();++it2){
			new_order[*it2].push_back(*it);	
		}
	}
}

void lexBFS_plus(graph & g, vector<int> & per, 
								vector<int> & inv, vector<int> & init){
	
	vector<vector<int> > reorderedGraph(g.n,vector<int>());	
	reorderNeighbourhoood(g.g,reorderedGraph, init);	
	
	PartRef pr(init);
	pr.partition(reorderedGraph);

	per = pr.per;
	inv = pr.inv;
}

void multiSweep(int k, graph & g, vector<int> & per, vector<int> & inv){
	int n = g.n; 
	vector<int> init(n,0);
	for(int i=0;i<n;++i){
		init[i] = i;
	}
	//TODO remove some inefficiencies 
	for(int i=0;i<k;++i){
		// lesBFS
		lexBFS_plus(g,per,inv,init);
		
		// BFS only
		//BFS_plus(g,init,per);
		
		init = per;
		reverse(init.begin(),init.end()); 

	
	}
}

// this is called RN in Habib et al.
void leftNeighbourhood(vector<vector<int> > & LN, graph & g, 
								vector<int> & per, vector<int> & inv){
	for(vector<int>::reverse_iterator rit = per.rbegin(); rit != per.rend(); ++rit){
		for(vector<int>::iterator it = g.g[*rit].begin(); it != g.g[*rit].end(); ++it){
			if(inv[*it] < inv[*rit]){
				LN[*it].push_back(*rit);
			}
		}
	}
}

bool checkLeftNeighbourhood(vector<int> & n1, vector<int> & n2){
	if(n1.size() + 1 != n2.size()) return false;
	for(int i=1;i<(int)n2.size();++i){
		if(n1[i-1] != n2[i]) return false;	
	}
	return true;
}

void calcCT(graph & g, cliqueTree & ct){
	int n = ct.n = g.n;	
	vector<int> per,inv;
	vector<vector<int> > LN(n,vector<int> ());

	lexBFS(g,per,inv);

	leftNeighbourhood(LN, g, per, inv);

	vector<vector<int> >  & cliques = ct.cliques;
	vector<pair<int,vector<int> > > & edges = ct.edges;
	vector<int> node_clique(n,-1);//clique_parents;
	
	int cur = 0;
	cliques.push_back(vector<int>());
	cliques[cur].push_back(per[0]);
	node_clique[per[0]] = 0;
	//clique_parents.push_back(-1);
	edges.push_back(make_pair(-1,vector<int>()));

	for(int i=1;i<n;++i){
		if(checkLeftNeighbourhood(LN[LN[per[i]][0]],LN[per[i]])){
			cliques[cur].push_back(per[i]);
			node_clique[per[i]] = node_clique[LN[per[i]][0]];
		}else{
			++cur;
			cliques.push_back(LN[per[i]]);
			cliques[cur].push_back(per[i]);	
			node_clique[per[i]] = cur;
			//clique_parents.push_back(node_clique[LN[per[i]][0]]);
			edges.push_back(make_pair(node_clique[LN[per[i]][0]],LN[per[i]]));
		}
	}
}
