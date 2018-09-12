#include "algo_cycles.hpp" 
#include "algo_BFS.hpp"

#include <queue>
#include <algorithm>

//debug
using namespace std;
#include <iostream>
#include <cassert>

/* checking if there is a large chord in a cycle
 * we permit chords of length k*/

bool largeChordTest(graph & g, int k, vector<int> cycle, int **dis){
	int n = g.n;
	int cs = cycle.size();
	vector<int> on(n,-1);
	for(int i=0;i<cs;++i){
		on[cycle[i]] = i;
	}
	for(int i=0;i<cs;++i){
		for(int j=i+1;j<cs;++j){
			int d = min(cs - j + i,j - i);
			if(d - dis[cycle[i]][cycle[j]] > k){
				return false;
			}
		}
	}

	return true;
}

void algo_cycles(graph & g, int d, int k,vector<int> & assign){
	int n = g.n;
	vector<int> par(n,-1),dep(n,-1);
	vector<int> cycle;
	int best = 2;

	cout << "init distance" << endl;

	int ** dis = new int*[n];
	for(int i=0;i<n;++i){
			dis[i] = new int[n];
	}

	cout << "distance start" << endl;

	BFS_ALL_DIS(g,dis);

	cout << "distance done\n" << endl; 

	queue<int> q;
	int x,u,v;
	//finding a large cycle
	for(int i=0;i<n;++i){

		while(!q.empty()) q.pop();
		fill(par.begin(),par.end(),-1);
		fill(dep.begin(),dep.end(),-1);
	
		q.push(i);
		dep[i] = par[i] = 0;
		while(!q.empty()){
			x = q.front();
			q.pop();
			for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
				assert( 0 <= *it && *it < n);
				//not yet dicovered 
				if(par[*it] == -1){
					q.push(*it);
					par[*it] = x;
					dep[*it] = dep[x] + 1;
				// finding closet closing in bfs tree
				}else{
					u = x;
					v = *it;
					if(dep[v] > dep[u]) swap(u,v);
					// largest possible cycle
					if(best < 2*dep[u]){
						int cur = u,cur2;
						vector<int> c1,c2;
						int de = dep[u];
						assert(dep[v] <= dep[u]);
						while(de != dep[v]){
							c1.push_back(cur);
							cur = par[cur];
							--de;
							assert(c1.size() <= n);
						}
						//same parent
						if(cur == v){
							c1.push_back(v);
						}else{
							cur2 = v;
							while(cur != cur2){
								c1.push_back(cur);
								c2.push_back(cur2);
								cur = par[cur];
								cur2 = par[cur2];
								assert(c1.size() <= n);
								assert(c1.size() <= n);
							}
							c1.push_back(cur);
							reverse(c2.begin(),c2.end());
							c1.insert(c1.end(),c2.begin(),c2.end());
						}
						if(best < (int)c1.size() && largeChordTest(g,k,c1,dis)){ 
							cycle = c1;
							best =c1.size();
						}
					}
					//cout << best << endl;
				}
			}
		}
		cout << cycle.size() << endl;
	}
	
	assign.resize(n);
	for(int i=0;i<n;++i) assign[i] = 0;
	for(int i=0;i<(int)cycle.size();++i){
		assign[cycle[i]] = 1;
	}
	vector<int> close;
	for(int i=0;i<n;++i){
		if(assign[i] == 0){
			fill(dep.begin(),dep.end(),-1);
			while(!q.empty()) q.pop();
			q.push(i);
			dep[i] = 0;
			while(!q.empty()){
				x = q.front();
				if(assign[x]){
					close.push_back(i);
					break;
				}
				q.pop();
				for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
					if(dep[*it] == -1 && dep[x] + 1 <= d){
						q.push(*it);
						dep[*it] = dep[x] + 1;
					}
				}
			}
		}
	}
	
	for(int i=0;i<(int)close.size();++i){
		assign[close[i]] = 1;
	}

	/*for(int i=0;i<n;++i){
		delete [] dis[i];
	}
	delete [] dis; */
}

