#include "algo.hpp"
#include "algo_interval.hpp"
#include "partition_refinement_interval.hpp"
#include "algo_BFS.hpp"

#include <algorithm>
#include <queue>
#include <cstring>

#include <cassert>
#include <iostream>

using namespace std;

void construct_vertices_in_clique(vector<vector<int> > & cliques, 
								vector<vector<int > > & v_contained_c){
	for(int i=cliques.size()-1;i>=0;--i){
		for(vector<int>::iterator it = cliques[i].begin(); it != cliques[i].end(); ++it){
			v_contained_c[*it].push_back(i);
		}
	}
}

void calculate_intervals(vector<vector<pair<int,int> > > & intervals,cliqueTree & ct){
	int n = ct.n;
	vector<vector<int > > & cliques = ct.cliques;	
	//from left to right from last to first clique discovered
	//reverse(cliques.begin(),cliques.end());

	vector<vector<int> > v_in_c(ct.n, vector<int>());
	vector<vector<int> > edges(cliques.size(), vector<int>());
	vector<vector<int> > seperators;
	vector<int> clique_order;

	//cout << cliques.size() << " " << ct.edges.size() << endl;

	//assert(edges.size() == ct.edges.size() + 1);

	//create mapping
	construct_vertices_in_clique(cliques, v_in_c);

	//create edges and seperators
	for(size_t i=1;i<ct.edges.size();++i){
		int parent = ct.edges[i].first;
		edges[parent].push_back(i-1);
		edges[i].push_back(i-1);
		seperators.push_back(ct.edges[i].second);
	}

	// partition
	PartRefInter pr(n,v_in_c, edges, seperators,cliques);
	pr.partition(clique_order);

	//reconstruct
	int * first = new int[ct.n];
	int * last = new int[ct.n];
	for(int i=0;i<n;++i){
		first[i] = last[i] = -1;
	}

	intervals.resize(n,vector<pair<int,int> >());
	for(size_t i=0;i<cliques.size();++i){
		int j = clique_order[i];
		for(vector<int>::iterator v = cliques[j].begin(); v != cliques[j].end(); ++v){
			if(first[*v] == -1){
				first[*v] = last[*v] = i;
			}else{
				if(last[*v] == (int)i-1){
					last[*v] = i;
				}else{
					intervals[*v].push_back(make_pair(first[*v],last[*v]));
					first[*v] = last[*v] = i;
				}
			}
		}
	}
	
	for(int i=0; i<n; ++i){
		intervals[i].push_back(make_pair(first[i],last[i]));
	}

	delete [] first;
	delete [] last;
}

bool check_interval(graph & g){
	cliqueTree ct;	
	calcCT(g,ct);
	
	vector<vector<pair<int,int > > > intervals;
	calculate_intervals(intervals,ct);

	for(vector<vector<pair<int,int> > >::iterator it = intervals.begin();
									it != intervals.end(); ++it){
		if(it->size() != 1) return false;
	}
	return true;	
}


int diametral_path(graph & g,int k){//vector<int> &path){
	int n = g.n, res = 0,tmp;
	vector<int> per,inv;
	//path.resize(n);

	multiSweep(k,g,per,inv);

	/*for(int i=0;i<n;++i){
		cout << per[i] + 1 << " ";
	}
	cout << endl;*/

	//int * par = new int[n];
	vector<int> par(n,0);
	//bool * diametral_p = new bool[n];	
	vector<bool> diametral_p(n,false);
	bool * used = new bool[n];
	
	assert( 0 <= per[0] && per[0] < n);
	BFS(g,per[0],par,tmp);

	//memset(diametral_p,false,sizeof(bool)*n);
	//for(int i=0;i<n;++i) diametral_p[i] = false;

	int cur = per[n-1];
	while(par[cur] != cur){
		if( cur >= n || cur < 0 ) cout << cur << endl;
		assert(cur < n && cur >= 0);
		diametral_p[cur] = true;
		cur = par[cur];
	}

	diametral_p[cur] = true;
	
	int c = 0;
	for(int i=0;i<n;++i){
		if(!diametral_p[i]){
			int tmp = BFS_diametral_path(g,i,used,diametral_p);
			assert(tmp != -1);
			res = max(res,tmp);
			++c;
		}
	}

	//cout << c + c3 + 1 << endl;

	/*for(int i=0;i<n;++i){
		if(!diametral_p[i]){
			path[i] = 0;
		}else{
			path[i] = 1;
		}
	}*/

	
	delete [] used;
	//delete [] diametral_p;	
	//delete [] par ;

	return res;
}



