#include "algo_chordless.hpp"

#include <stack>
#include <cassert>
//DEBUG
#include <iostream>
#include "test.hpp"

using namespace std;

void chordless_cycles(graph & g, vector<vector<int> >& cycles){
	assert(g.n <= 100);

	int n = g.n;
	int l;
	vector<vector<bool> > mat(n,vector<bool>(n,0));
	vector<int> path(n,-1);
	vector<int> length(n,0);
	vector<bool> on_path(n,0);

	for(int i=0;i<n;++i){
		for(int j=0;j<(int)g.g[i].size();++j){
			mat[i][g.g[i][j]] = 1;
		}
	}

	for(int i=0;i<n;++i){
		path[0] = i;
		length[0] = -1;
		on_path[i] = 1;
		l = 1;

		auto backtrack = [&](){
			//backtrack
			while(l && length[l-1] == (int)g.g[path[l-1]].size()){
				on_path[path[l-1]] = 0;
				path[l-1] = -1;
				--l;
			}
			if(l == 0) return true;
			length[l-1]++;
			return false;
		};

		while(l > 0){
			if(backtrack()) break;
			while(length[l-1] < (int)g.g[path[l-1]].size() && (on_path[g.g[path[l-1]][length[l-1]]] || g.g[path[l-1]][length[l-1]] <= i)){
				length[l-1]++;
			}
			//cout << length[l-1] << " " << g.g[path[l-1]].size() << " " << path[l-1] << " " << on_path[g.g[path[l-1]][length[l-1]]] << " " << g.g[path[l-1]][length[l-1]] << endl;
			if(length[l-1] == (int)g.g[path[l-1]].size()) continue;

			int ext = g.g[path[l-1]][length[l-1]];

			//check for chords ( new l > 3)
			if(l > 2){
				bool check = false;
				for(int j=1;j<l-1;++j){
					if(mat[ext][path[j]]){
						check = true;
					}
				}
				if(check){
					length[l-1]++;
					continue;
				}
			}

			path[l] = ext;
			length[l] = -1;
			on_path[ext] = 1;
			++l;

			if(l > 2 && mat[i][path[l-1]]){
				//removing symmetries
				if(path[1] < path[l-1]){
					//TODO remove after debugging
					for(int o=0;o<l;++o) assert(path[o] != -1);
					vector<int> c(path.begin(),path.begin()+l);
					cycles.push_back(c);
					//print_vector(c);
				}
				--l;
				on_path[path[l]] = 0;
				path[l] = -1;
				length[l-1]++;
			}

			assert(l <= n);
		}
	}
}

