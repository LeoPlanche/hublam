#include "algo_branches.hpp"
#include "algo_BFS.hpp"

#include <cassert>

using namespace std;

bool branches(graph & g, int d ,vector<int> & assign){
	int n = g.n;
	assign.resize(n,0);
	
	int ** dis = new int*[n];
	bool* used = new bool[n];
	for(int i=0;i<n;++i){
		dis[i] = new int[n];
	}

	BFS_ALL_DIS(g,dis);

	bool found = false;
	int resi;
	for(int i=0;i<n;++i){
		vector<int> res;
		BFS_depth(g,i,d,res);
		if(res.size() < 3) continue;
		int s = res.size();
		for(int j=0;j<s;++j){
			for(int k=j+1;k<s;++k){
				if(dis[res[j]][res[k]] >= 2*d){
					assert(dis[res[j]][res[k]] == 2*d);
					for(int l=k+1;l<s;++l){
						if(dis[res[j]][res[l]] >= 2*d && dis[res[l]][res[k]] >= 2*d){
							found = true;
							assign[res[j]] = assign[res[k]] = assign[res[l]] = 1;
							break;
						}
					}
				}
				if(found) break;
			}
			if(found) break;
		}
		if(found){
			resi = i;
			break;
		}
	}
	if(!found) return false;
	
	BFS_short_circuited(g,resi,used,d/5);
	
	for(int i=0;i<n;++i){
		assign[i] =(int) used[i];
	}
	
	for(int i=0;i<n;++i){
		delete [] dis[i];
	}
	delete [] dis;
	delete [] used;

	return true;
}

