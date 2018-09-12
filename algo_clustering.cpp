#include "algo_clustering.hpp"
#include "dsu.hpp"
#include "algo_BFS.hpp"

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>

struct node{
	int id;
	vector<int> lex;
	
	bool operator < (const node & rhs){
		int i = lex.size()-1;
		int j = rhs.lex.size()-1;
		while(i>=0 && j>=0 && lex[i] == rhs.lex[j]){
			--i;--j;
		}
		if(i>=0 && j>=0) return (lex[i] < rhs.lex[j]);
		if(i>=0) return true;
		return false;
	}
};

/* TODO 
 * remove lazy deletion
 * dangerous on large graphs
 */

void lexDFS(graph & g, int start, vector<int> & per, vector<int> & inv){
	int n = g.n;
	for(int i=0;i<n;++i){
		inv[i] = -1;
	}
	
	int i=0;
	stack<int> s;	
	s.push(start);
	vector<vector<int> > lexs(n,vector<int> ());

	auto local_compare = [&] (const int a, const int b){
		int i = lexs[a].size()-1;
		int j = lexs[b].size()-1;
		while(i>=0 && j>=0 && lexs[a][i] == lexs[b][j]){
			--i;--j;
		}
		if(i>=0 && j>=0) return (lexs[a][i] < lexs[b][j]);
		if(i>=0) return true;
		return false;
	};
	
	while(!s.empty()){

		// lazy part
		while(!s.empty() && inv[s.top()] != -1) s.pop();
		if(s.empty()) break;

		int v = s.top();
		s.pop();
		per[i] = v;
		inv[v] = i;
		vector<int> update;
		for(vector<int>::iterator it = g.g[v].begin();
										it != g.g[v].end(); ++it){
			if(inv[*it] == -1){
				update.push_back(*it);
			}
		}
		sort(update.begin(),update.end(),local_compare);
		for(vector<int>::iterator it = update.begin();
										it != update.end(); ++it){
			s.push(*it);
		}
		++i;
	}
}

void sort_edges_betweeness(graph & g,edges & e,int l,int * edge_order){
	int n = g.n, m = g.m;
	vector<int> per(n),inv(n);
	double * scores  = new double[m];

	for(int i=0;i<m;++i) edge_order[i] = i;

	for(int i=0;i<l;++i){
		//BFS(g,rand() % n, size,last);
		lexDFS(g,rand()%n,per,inv);
		for(int j=0;j<m;++j){
			double s = 1.0 - (double)(abs(inv[e.u[j]] - inv[e.v[j]]))/(double)(g.m);
			scores[j]  = (scores[j] * i + s)/(double)(i+1);
		}
	}

	auto local_compare = [&] (const int a, const int b){
		return scores[a] > scores[b];
	};

	sort(edge_order,edge_order+m,local_compare);

	delete [] scores;
}


void compactCommunities(graph & g, edges & e, int l, int & nbCommunities, vector<int>& assign){
	int n = g.n;
	long long m = g.m;
	assign.resize(n);

	struct ed{
		int id;	
		list<ed>::iterator rev;
		ed(int id) : id(id){};
		int getId(){ return id;}
		void update(list<ed>::iterator it){
			this->rev = it;
		}
		list<ed>::iterator getIt(){
			return rev;
		}
	};

	long long * coverage   = new long long[n];
	long long * volume 	   = new long long [n];
	int * size = new int[n];
	list<ed> * edge_list = new list<ed>[n];
	int * edge_order = new int[m];

	for(int i=0;i<n;++i){
		size[i] = 1;
		coverage[i] = 0;
		volume[i] = (int)g.g[i].size();
	}

	for(int i=0;i<m;++i){
		int x = e.u[i];
		int y = e.v[i];
		edge_list[y].push_back(ed(x));
		edge_list[x].push_back(ed(y));
		list<ed>::iterator it1,it2;
		it1 = --edge_list[x].end();
		it2 = --edge_list[y].end();
		edge_list[x].back().update(it2);
		edge_list[y].back().update(it1);
	}

	sort_edges_betweeness(g,e,l,edge_order);

	double modularity = 0.0;
	for(int i=0;i<n;++i){
		modularity -= (((double) volume[i] * volume[i])/((double)4*m*m));
	}

	int i=0,mai = -1;
	double maxi = -1.0;
	DSU dsu(n);

	while(i < m){
		//assert( modularity > -2.0 && modularity < 2.0);
		int x = dsu.find(e.u[edge_order[i]]);
		int y = dsu.find(e.v[edge_order[i]]);

		if(x != y){
			modularity -= ((double)coverage[x]/(double)m) - 
										 (((double) volume[x] * volume[x])/(double)(4*m*m));
			modularity -= ((double)coverage[y]/(double)m) - 
										 (((double) volume[y] * volume[y])/(double)(4*m*m));
			
			int small,big;
			if(edge_list[x].size() < edge_list[y].size()){
				small = x;
				big = y;
			}else{
				small = y;
				big = x;
			}

			list<ed>::iterator it = edge_list[small].begin();
			while(it != edge_list[small].end()){
				if(dsu.find(it->getId()) == big){
					edge_list[big].erase(it->getIt());
					it = edge_list[small].erase(it); 
					++coverage[big];
				}else{
					++it;
				}
			}

			edge_list[big].splice(edge_list[big].end(),edge_list[small]);
			volume[big] += volume[small];
			dsu.merge(small,big);

			assert(edge_list[small].size() == 0);
			assert(dsu.find(small) == big);
			
			modularity += ((double)coverage[big]/(double)m) - 
										 (((double) volume[big] * volume[big])/(double)(4*m*m));
		}

		cout << modularity << endl;

		if(maxi < modularity){
			mai = i;
			maxi = modularity;
		}
		++i;
	}
	
	// second run
	i = 0;
	dsu.reinit();
	nbCommunities = n;
	int mac = 0;
	
	while(i < mai){
		int x = dsu.find(e.u[edge_order[i]]);
		int y = dsu.find(e.v[edge_order[i]]);
		if(x != y){
			size[y] += size[x];	
			nbCommunities--;
			dsu.merge(x,y);
			if(size[mac] < size[y]) mac = y;
		}
		/*int px = dsu.find(x);
		int py = dsu.find(y);
		if(px != py){
			size[py] += size[px];
			dsu.merge(x,y);
			nbCommunities--;
		}*/
		++i;
	}

	cout << nbCommunities << endl;

	map<int,int> col;
	//int j = 0;

	for(int i=0;i<n;++i){
		/*if(col.find(dsu.find(i)) == col.end()){
			col[dsu.find(i)] = j;
			++j;
		}
		assign[i] = col[dsu.find(i)];	*/
		if(dsu.find(i) == mac) assign[i] = 1;
		else assign[i] = 0;
	}
	//assert(j == nbCommunities);
	nbCommunities = 2;
	
	delete [] size;
	delete [] edge_list;
	delete [] edge_order;
	delete [] volume;
	delete [] coverage;
}


void compactCommunities2(graph & g, graph_edge_nb & g1, edges & e, int l, int & nbCommunities, vector<int>& assign){
	int n = g.n, m = g.m;
	assign.resize(n);
	
	int  * edge_order = new int[m];
	bool * forbidden = new bool[m];
	bool * seen = new bool[n];

	for(int i=0;i<m;++i){
		forbidden[i] = false;
	}

	sort_edges_betweeness(g,e,l,edge_order);

	auto BFS_forbidden = [&](){
		for(int i=1;i<n;++i) seen[i] = false;
			
		queue<int> q;
		q.push(0);
		seen[0] = true;
		int c = 1;
		while(!q.empty()){
			int x= q.front();
			q.pop();
			for(vector<pair<int,int> >::iterator it = g1.g[x].begin(); it != g1.g[x].end(); ++it){
				int y = it->first;
				int e = it->second;
				if(!seen[y] && !forbidden[e]){
					seen[y] = true;
					q.push(x);
					++c;
				}
			}
		}
		return c == n; 
	};

	int i=m-1;
	while(true){
		forbidden[edge_order[i]] = true;
		if(!BFS_forbidden()) break;
		--i;
	}

	nbCommunities = 2;
	for(int i=0;i<n;++i){
		if(seen[i]) assign[i] = 1;
		else assign[i] = 0;
	}
				
	delete [] forbidden;
	delete [] edge_order;
}

/* TODO
 *Maybe do newman with same approach as cruesefond
 */

void newman_greedy_clustering(graph & g, edges & e, int & nbCommunities, vector<int>& assign){
	assert(g.n <= 10000);

	int n = g.n;
	long long m = g.m;
	assign.resize(n);
	DSU dsu(n);

	long long ** ma = new long long *[n];
	long long * volume = new long long[n];
	for(int i=0;i<n;++i){
		ma[i] = new long long[i+1];
		volume[i] = (long long) g.g[i].size();
	}
	
	for(int i=0;i<n;++i){
		for(int j=0;j<=i;++j) ma[i][j] = 0;
	}

	for(int i=0;i<m;++i) 
		ma[max(e.u[i],e.v[i])][min(e.u[i],e.v[i])]++;

	double modularity = 0.0;
	for(int i=0;i<n;++i){
		modularity -= ((double)(volume[i] * volume[i]) / (double)(4 * m * m));
	}

	cout << modularity << endl;

	int mai = -1;
	double maMod = -1.0;
	vector<int> res;

	for(int i=0;i<n-1;++i){
		double currentmax = -1.0;
		int pos = -1;
		for(int j=0;j<m;++j){
			int pu = dsu.find(e.u[j]);	
			int pv = dsu.find(e.v[j]);
			int u = max(pu,pv);
			int v = min(pu,pv);
			if(u != v){
				double tmp = modularity + 2*(((double)ma[u][v]/(double)m) - 
										(((double)volume[u]/(double)(2*m))* ((double)volume[v]/(double)(2*m))));
				if(currentmax < tmp){
					pos = j;
					currentmax = tmp;
				}
			}
		}
		res.push_back(pos);
		int pu = dsu.find(e.u[pos]);	
		int pv = dsu.find(e.v[pos]);
		//int u = max(pu,pv);
		//int v = min(pu,pv);

		modularity = currentmax;

		//modularity += 2*((double)(ma[u][v]/(double)m) - 
		//								(((double)volume[u]/(double)2*m) * ((double)volume[v]/(double)2*m)));

		volume[pv] = (volume[pv] + volume[pu] + 2*ma[max(pv,pu)][min(pv,pu)]);
		int tmp = 0;
		for(int i=0;i<n;++i){
			if(i != pv && i != pu){
				ma[max(i,pv)][min(i,pv)] += ma[max(i,pu)][min(i,pu)];
				if(dsu.find(i) == i){
					tmp += ma[max(i,pv)][min(i,pv)];
				}
			}
		}
		ma[pv][pv] = ma[pu][pu] + ma[pv][pv] + ma[max(pu,pv)][min(pu,pv)];
		tmp += 2*ma[pv][pv];
		//volume[pv] = tmp;
		//assert(volume[pv] == tmp);

		dsu.merge(pu,pv);
		if(maMod < modularity){
			mai = i;
			maMod = modularity;
		}
		cout << modularity << endl;
	}
	
	dsu.reinit();

	for(int i=0;i<mai;++i){
		dsu.merge(e.u[res[i]],e.v[res[i]]);
	}

	nbCommunities = n - mai;
	map<int,int> mapi;
	int j = 0;
	for(int i=0;i<n;++i){
		if(mapi.find(dsu.find(i)) == mapi.end()){
			mapi[dsu.find(i)] = j;
			++j;
		}
		assign[i] = mapi[dsu.find(i)];
	}
	assert(nbCommunities == j);
	assert(mapi.size() == j);

	for(int i=0;i<n;++i){
		delete [] ma[i];
	}
	delete [] volume;
	delete [] ma;
}
