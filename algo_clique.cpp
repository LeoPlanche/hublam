#include "algo_clique.hpp"
#include "algo_BFS.hpp"

#include <cassert>
#include <iostream>

const int INF = 1e9;

using namespace std;

int diameter(graph & g){
	int n = g.n, res = 0, last;
	//int * par = new int[n];
	vector<int> par(n,0);

	//n BFS
	for(int i=0;i<n;++i){
		res = max(res,BFS(g,i,par,last));
	}

	//delete [] par;

	return res;
}

bool diameter_check(graph & g,int k){
	int n = g.n;
	bool * used = new bool[n];
	
	//n BFS
	for(int i=0;i<n;++i){
		if(BFS_short_circuited(g,i,used,k) > k) return false;
	}

	delete [] used;

	return true;
}

void FOURsweep(graph & g, int & u, int & lb){
	//maybe choose node with highest degree
	int r1 = 0,r2,last,lb1,lb2,d1,d2;
	//int * par = new int[g.n];
	vector<int> par(g.n,0);

	lb1 = BFS(g,r1,par,last);
	d1  = BFS(g,last,par,r2);

 	int tmp = 0;
	while(tmp < d1/2){
		r2 = par[r2];	
		++tmp;
	}

	lb2 = BFS(g,r2,par,last);		
	d2 	= BFS(g,last,par,u);
	
	tmp = 0;
	while(tmp < d2/2){
		u = par[u];
		++tmp;
	}

	lb = max(lb1,lb2);
}

int iFUB(graph & g, int u, int l, int k){
	int tmp;
	//int * par = new int[g.n];
	vector<int> par(g.n,0);
	stack<pair<int,int> > s;

	int i = BFS_tree(g,u,par,s);
	int lb = max(i,l);
	int ub = 2*i;
	
	while( ub - lb > k ){
		assert(s.top().second == i);
		if( lb > 2*(i-1) ) return lb;
		while(s.top().second == i){
			lb = max(lb,BFS(g,s.top().first,par,tmp));
			s.pop();
		}
		if( lb > 2*(i-1) ) return lb;
		ub = 2*(i-1);	
		--i;
	}

	//delete [] par;

	return lb;
}

int diameter_iFUB(graph & g){
	int u,lb;

	FOURsweep(g,u,lb);

	return iFUB(g,u,lb,0);
}
