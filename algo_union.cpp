#include "algo_union.hpp"
#include "algo_interval.hpp"
#include "GraphIO.hpp"
#include "dsu.hpp"
#include "algo_chordless.hpp"


#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <tuple>
#include <functional>
#include <iostream> 
#include <string>

#include <cassert>
#include <iostream>

using namespace std;

void quotient_graph(edges & e, graph & co_g, int co, const vector<int> &member){
	int n = co,m = 0;
	co_g.n = n;
	co_g.g.resize(n);
	vector<vector<bool> > mat(n,vector<bool>(n));
	for(int i = 0;i < e.m; ++i){
		int x = member[e.u[i]];
		int y = member[e.v[i]];
		if(x != y && !mat[x][y]){
			mat[x][y] = mat[y][x] = 1;
			co_g.g[x].push_back(y);
			co_g.g[y].push_back(x);
			++m;
		}
	}
	co_g.m = m;
}

void create_subgraph(graph & subg,const edges & e,
								const vector<int> & communities, const vector<int> &member){
		int & n = subg.n;
		int & m = subg.m;
		map <int,int> corres;

		auto check_membership = [&](const int node_id){
			for(vector<int>::const_iterator it = communities.begin();
											it != communities.end(); ++it){
				if(*it == member[node_id]) return true;
			}
			return false;
		};

		auto check_correspondance = [&](const int node_id){
			if(corres.find(node_id) == corres.end()){
				corres[node_id] = n;
				++n;
				subg.g.push_back(vector<int> ());
			}
		};
		
		for(int i=0;i<m;++i){
			int u = e.u[i];
			int v = e.v[i];
			if(check_membership(u) && check_membership(v)){
				check_correspondance(u);
				check_correspondance(v);
				int uu = corres[u];
				int vv = corres[v];
				subg.g[uu].push_back(vv);
				subg.g[vv].push_back(uu);
				subg.m++;
			}
		}
}

template <typename T> void unifying_laminars(const graph & g,const edges & e,double density_qc, int laminar_index,
								int &co, vector<int>& member, int unify_type, CovId & ci){
	int n = g.n;
	int m = g.m;
	int nco = co;
	vector<bool> done(nco,false);
	vector<vector<T> > mat(vector<T>(nco,0));
	priority_queue<tuple<T,int,int>> pq(greater<tuple<T,int,int>> );

	vector<int> vcount(co,0),ecount(co,0);

	for(int i=0;i<n;++i){
		assert(0 <= member[i] && member[i] < co);
		vcount[member[i]]++;
	}

	for(int i=0;i<m;++i){
		int c1 = member[e.u[i]];
		int c2 = member[e.v[i]];
		if(c1 != c2){
			switch(unify_type){
				case 0:
					mat[c1][c2]++;
					mat[c2][c1]++;
					break;
				
				case 1:
					mat[c1][c2] += ci.covs[i];
					mat[c2][c1] += ci.covs[i];
					break;

				case 2:{
					double tmp = ci.covs[i] * ci.ids[i];
					mat[c1][c2] += tmp;
					mat[c2][c1] += tmp;
					break;
				}

				default:
					cerr << "Error occured when calling unifying_laminars\n";
			}
		}else{
			ecount[c1]++;
		}
	}

	// leaves quasi-cliques alone
	for(int i=0;i<co;++i){
		int cn = (vcount[i]*(vcount[i] -1))/2;
		if((double)ecount[i]/(double)cn >= density_qc){
			done[i] = true;
		}
	}
	
	const T eps = numeric_limits<T>::epsilon();
	for(int i=0;i<nco;++i){
		for(int j=0;j<i;++j){
			if(!done[i] && !done[j] && mat[i][j] > eps){
				pq.push(make_tuple(mat[i][j],i,j));
			}
		}
	}
	
	while(!pq.empty()){
		T tmp;
		int x,y;
		tie(tmp,x,y) = pq.front();
		pq.pop();
	
		if(!done[x] && !done[y]){
			graph subg;
			vector<int> communities = {x,y};

			create_subgraph(subg,e,communities,member);
		
			int laminarity= diametral_path(subg,3);
			if(laminarity < laminar_index){
				for(int i=0;i<co;++i){
					mat[x][i] += mat[y][i];
					mat[i][x] += mat[y][i];
					if(!done[i] && mat[x][i] > eps){
						pq.push(make_tuple(mat[x][i],x,i));
					}
				}
				for(int i=0;i<n;++i){
					if(member[i] == y) member[i] = x;
				}
				done[y] = true;
				--nco;
			}
		}
	}
	co = nco;
}

void output_cycles(graph & g, edges & e, int & co, vector<int> & member, string output){
	graph qg;
	vector<vector<int> > cycles;

	quotient_graph(e,qg,co,member);
	chordless_cycles(qg,cycles);
	
	for(int i=0;i<(int)cycles.size();++i){
		sort(cycles[i].begin(),cycles[i].end());
		vector<int> nco(g.n,0);
		for(int j=0;j<g.n;++j){
			if(binary_search(cycles[i].begin(),cycles[i].end(),member[j])){
				nco[j] = 1;
			}
		}
		string out = (output + ".cycle" + "to_string(i)" + ".gdf"); 
		const char * tmp_array = out.c_str();
		create_gephi_coloring(e,tmp_array,2,nco);
	}
}
