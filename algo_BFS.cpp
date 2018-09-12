#include "algo_BFS.hpp"
#include "algo.hpp"

#include <queue>
#include <stack>
#include <cstring>

//debug
#include <iostream>
#include <cassert>

int BFS(graph & g, int start, vector<int> & par, int & last){
	int n = g.n,x;
	for(int i=0; i<n; ++i) par[i] = -1;
	vector<int> dis(n,0);

	int count = 0;
	
	queue<int> q;
	q.push(start);
	par[start] = start;
	while(!q.empty()){
		x = q.front();
		q.pop();
		for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
			if(par[*it] == -1){
				par[*it] = x;
				dis[*it] = dis[x] + 1;
				q.push(*it);
			}
		}
		++count;
	}
	
	cout << count << " " << n << endl;
	assert(count == n);
	
	for(int i=0;i<n;++i){
		assert(par[i] != -1);
	}

	last = x;
	return dis[last];
}

void BFS_depth(graph & g, int start, int depth, vector<int> & res){
	int n = g.n,x,di;
	int * dis = new int[n];
	for(int i=0;i<n;++i) dis[i] = -1;

	queue<int> q;
	q.push(start);
	dis[start] = 0;
	while(!q.empty() && dis[q.front()] < depth ){
		x = q.front();
		q.pop();
		di = dis[x];
		for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
			if(dis[*it] == -1){
				q.push(*it);
				dis[*it] = di + 1;
			}
		}
	}

	while(!q.empty()){
		assert(dis[q.front()] == depth);
		res.push_back(q.front());
		q.pop();
	}

	delete [] dis;
}

void BFS_ALL_DIS(graph & g, int ** dis){
	int n = g.n,x,di;

	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j) dis[i][j] = -1;
		queue<int> q;
		q.push(i);
		dis[i][i] = 0;
		while(!q.empty()){
			x = q.front();
			q.pop();
			di = dis[i][x];
			for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
				if(dis[i][*it] == -1){
					q.push(*it);
					dis[i][*it] = di + 1;
				}
			}
		}
	}
}

int BFS_tree(graph & g, int start, vector<int> & par, stack<pair<int,int> > & s){
	int n = g.n,x,di;
	for(int i=0; i<n; ++i) par[i] = -1;
	
	queue<pair<int,int> > q;
	q.push(make_pair(start,0));
	s.push(make_pair(start,0));
	par[start] = start;
	while(!q.empty()){
		pair<int,int> p = q.front();
		q.pop();
		x = p.first;
		di = p.second;
		for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
			if(par[*it] == -1){
				par[*it] = x;
				q.push(make_pair(*it,di+1));
				s.push(make_pair(*it,di+1));
			}
		}
	}
	return di;
}

BFSTree BFS_tree_full(graph & g, int start){

        BFSTree t;

	t.n = g.n;      
        int tab[t.n];
        int tab2[t.n];
        int invTab[t.n];
        int k =0;
        t.voisins.resize(g.n);
	for(int i=0; i<t.n; ++i) {
            t.status.push_back(1);
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
             tab[i]=-1;
             invTab[i]=-1;
             tab2[i]=-1;}
        

         
	
	queue<int> q;
	q.push(k);
        tab[k]=start;
        invTab[start]=k;
        t.pere[k] = k;
        tab2[start]=0;
	while(!q.empty()){

		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g.g[tab[p]].begin(); it != g.g[tab[p]].end(); ++it){
                          if(tab2[*it] ==-1){
                                 tab2[*it] = 0;
                                k++;
                                tab[k]=*it;
                                invTab[*it]=k;
				t.pere[k] = p;
                                t.fils[p].push_back(k);
				q.push(k);

			}
		}
	}
   
        for(int i =0; i<t.n;i++)
        {
           for(vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end(); ++it){
               if((t.pere[invTab[i]]==invTab[*it]) || (t.pere[invTab[*it]]==invTab[i]))             
               t.voisins[invTab[i]].push_back(make_pair(invTab[*it],1));             
               else
               t.voisins[invTab[i]].push_back(make_pair(invTab[*it],-1));
              
           }         
        }
    
	return t;
}

BFSTreeReel BFS_tree_reel(graph & g, int start){

        BFSTreeReel t;
	t.n = g.n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(start); 	
        t.pere[start] = start;
        t.profondeur[start] = 0;	
        
        while(!q.empty()){
		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g.g[p].begin(); it != g.g[p].end(); ++it){
                          if(t.pere[*it] ==-1){
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;
                                t.fils[p].push_back(*it);
				q.push(*it);

			}
		}
	}
	return t;
}

int BFS_short_circuited(graph & g, int start, bool * used, int k){
	int n = g.n,x,di;
	memset(used,0,sizeof(bool) * n);
	
	queue<pair<int,int> > q;
	q.push(make_pair(start,0));
	used[start] = true;
	while(!q.empty()){
		pair<int,int> p = q.front();
		q.pop();
		x = p.first;
		di = p.second;
		for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
			if(!used[*it]){
				if(di + 1 > k) return di + 1;
				used[*it] = true;
				q.push(make_pair(*it,di+1));
			}
		}
	}
	return di;
}

int BFS_diametral_path(graph & g, int start, bool * used , const vector<bool> & diametral_p){
	int n = g.n;
	memset(used,0,sizeof(bool) * n);

	/*vector<bool> copy(diametral_p,diametral_p+n);
	for(int i=0;i<n;++i){
		if(copy[i] != diametral_p[i]) 
			cout << "big error" << endl;
	}*/

	if(diametral_p[start]) return 0;

	queue<pair<int,int> > q;
	q.push(make_pair(start,0));
	used[start] = true;
	while(!q.empty()){
		pair<int,int> p = q.front();
		q.pop();
		int x = p.first;
		int di = p.second;
		for(vector<int>::iterator it = g.g[x].begin(); it != g.g[x].end(); ++it){
			if(diametral_p[*it]) return di + 1;
			if(!used[*it]){
				used[*it] = true;
				q.push(make_pair(*it,di+1));
			}
		}
	}
	// should not happen
	return -1;
}


void BFS_plus(graph & g, vector<int> & per, vector<int> & res){
	int n = g.n,x,i=0;
	vector<int> seen(n,-1);
	res.resize(n);
	vector<vector<int> > reorderG(n,vector<int> ());
	reorderNeighbourhoood(g.g,reorderG,per);
	
	queue<int> q;
	q.push(per[0]);
	while(!q.empty()){
		x = q.front();
		q.pop();
		for(vector<int>::iterator it = reorderG[x].begin(); it != reorderG[x].end(); ++it){
			if(seen[*it] == -1){
				res[i] = *it;
				++i;
				seen[*it] = 0;
				q.push(*it);
			}
		}
	}
}

