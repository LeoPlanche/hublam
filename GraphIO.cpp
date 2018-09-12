#include "GraphIO.hpp"
#include "utility.hpp"
#include <stdlib.h> 


#include <time.h>       /* time */
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <cmath>

#include <algorithm>

//debug
#include <cassert>
#include <queue>

using namespace std;

bool read_graph(graph & g,const char * input){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	is >> g.n >> g.m;
        g.g.clear();
	g.g.resize(g.n);
	for(int i=0; i<g.m;++i){
		int x,y;
		is >> x >> y;
		g.g[x].push_back(y);
		g.g[y].push_back(x);
	}
	is.close();
	return true;
}

bool write_graph(graph & g,const char * input){
	ofstream of(input);
        if(!of.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	 of << g.n << " " << g.m << endl;
	for(int i=0; i<g.n;++i){
            for (int j =0; j<g.g[i].size(); j++)
            {               
              if(i<= g.g[i][j])
              of << i << " " <<g.g[i][j]  << endl;
            }
	}
	of.close();
	return true;
}

bool write_squelette(squelette & g,const char * input){
    ofstream of(input);
        if(!of.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
    
      of << "graph graphname {" << endl;
         for(int i=0; i<g.n;++i){
             if(g.hub[i]!=-1){
                 of<< i << " [color=red];"<< endl;}
             else {      
                   of<< i << " [color=blue];"<< endl;} 
                                  }
    
	for(int i=0; i<g.n;++i){
            for (int j =0; j<g.g[i].size(); j++)
            {               
              if(i<= g.g[i][j])
              {
                 if(g.hub[i]!=-1 && g.hub[g.g[i][j]]!=-1 ){
              of << i << " -- " <<g.g[i][j] << " [color=red];" << endl;
              } else if(g.hub[i]==-1 && g.hub[g.g[i][j]]==-1 )
              {
                 of << i << " -- " <<g.g[i][j] << " [color=blue];" << endl; 
              }
              else
              {
               of << i << " -- " <<g.g[i][j] << " [color=purple];" << endl;  
              }
              }
            }
	}
            of << "}" << endl;
	of.close();
        return true;
}

bool read_graph_edge_nb(graph_edge_nb & g, char * input){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	is >> g.n >> g.m;
	g.g.resize(g.n);
	for(int i=0; i<g.m;++i){
		int x,y;
		is >> x >> y;
		g.g[x].push_back(make_pair(y,i));
		g.g[y].push_back(make_pair(x,i));
	}
	is.close();
	return true;
}

int assign_get(map<int,int> & m, int & cur, int val){
	int res;
	if(m.find(val) == m.end()){
		res = m[val] = cur;
		++cur;
	}else{
		res = m[val];	
	}
	return res;
}

bool read_cc(mapping & m, char * input){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	graph & g = m.g;
	map<int,int> & ma = m.m;
	edges & e = m.e;

	is >> g.n >> g.m;
	e.n = g.n;
	e.m = g.m;
	g.g.resize(g.n);
	e.u.resize(e.m);
	e.v.resize(e.m);
	int cur = 0;
	for(int i=0; i<g.m;++i){
		assert(cur <= g.n);
		int x,y,mx,my;
		is >> x >> y;
		mx = assign_get(ma,cur,x);
		my = assign_get(ma,cur,y);
		e.u[i] = mx;
		e.v[i] = my;
		g.g[mx].push_back(my);
		g.g[my].push_back(mx);
	}
	is.close();
	return true;
}

bool read_cc_edge_nb(mapping_edge_nb & m, char * input){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	graph_edge_nb & g = m.g;
	map<int,int> & ma = m.m;
	edges & e = m.e;

	is >> g.n >> g.m;
	e.n = g.n;
	e.m = g.m;
	g.g.resize(g.n);
	e.u.resize(e.m);
	e.v.resize(e.m);
	int cur = 0;
	for(int i=0; i<g.m;++i){
		assert(cur <= g.n);
		int x,y,mx,my;
		is >> x >> y;
		mx = assign_get(ma,cur,x);
		my = assign_get(ma,cur,y);
		e.u[i] = mx;
		e.v[i] = my;
		g.g[mx].push_back(make_pair(my,i));
		g.g[my].push_back(make_pair(mx,i));
	}
	is.close();
	return true;
}

bool read_edges(edges & e, char * input){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	is >> e.n >> e.m;
	e.u.resize(e.m);
	e.v.resize(e.m);
	for(int i=0; i<e.m;++i){
		is >> e.u[i] >> e.v[i];
	}
	is.close();
	return true;
}

void split_string(string s,char delim, vector<string> & elems){
	stringstream ss(s);
	string item;
	while(getline(ss,item,delim)){
		elems.push_back(item);	
	}
}

bool read_graph_coloring(graph & g, edges & e, int &nb, vector<int> &col, char * input, int k){
	assert(0 <= k && k < 7);
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	map<int,int> m;
	string line;

	is >> g.n >> g.m;
	e.n = g.n;
	e.m = g.m;
	col.resize(g.n);
	g.g.resize(g.n);
	
	getline(is,line);
	
	int c,j=0;
	for(int i=0;i<g.n;++i){
		getline(is,line);
		vector<string> parts;	
		split_string(line,',',parts);
		
		assert(parts.size() == 9);

		c = atoi(parts[2+k].c_str());

		if(m.find(c) == m.end()){
			m[c] = j;
			++j;
		}
		col[i] = m[c];
	}

	nb = j;
	for(int i=0;i<g.m;++i){
		getline(is,line);
		vector<string> parts;	
		split_string(line,',',parts);
	
		//cout << i << " " << parts.size() << endl;
		assert(parts.size() == 11);
		
		int x = atoi(parts[0].substr(1,parts[0].size()).c_str());
		int y = atoi(parts[1].substr(1,parts[1].size()).c_str());
		g.g[x].push_back(y);
		g.g[y].push_back(x);
		e.u.push_back(x);
		e.v.push_back(y);
	}
	return true;
}

bool get_maxCover_and_id(const char * input, CovId & ci){
	ifstream is(input);
	if(!is.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}

	vector<double> & covers = ci.covs;
	vector<double> & identities = ci.ids;
	
	string line;
	getline(is,line);
	while(getline(is,line)){
		vector<string> parts;
		split_string(line,'\t',parts);
	
		assert(parts.size() == 12);
		double cover = 0.0;
		for(int i=0;i<2;++i){
			int vals[3];
			for(int j=0;j<3;++j){
				vals[j] = atoi(parts[2+4*i+j].c_str());
			}
			cover = max(cover,(double)(abs(vals[0]- vals[1]) + 1)/(double)(vals[2]));
		}
		covers.push_back(cover);
		identities.push_back(atof(parts[10].c_str())/100.0);
	}
	return true;
}

bool create_gephi_coloring(edges & e, const char * input, 			int nbCommunities, vector<int>& assign){
	ofstream of(input);
	if(!of.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	
	int j=0,n = e.n;
	map<int,int> m;
	vector<Color> colors;
	generate_colors(colors,nbCommunities,1);

	of << "nodedef>name INF,color VARCHAR" << endl;

	for(int i=0;i<n;++i){
		if(m.find(assign[i]) == m.end()){
			m[assign[i]] = j;
			++j;
		}
		int cur = m[assign[i]];
		of << i << ",'" << colors[cur] << "'" << endl;
	}

	of << "edgedef>node1 INT,node2 INT" << endl;

	for(int i=0;i<e.m;++i){
		of << e.u[i] << "," << e.v[i] << endl;
	}

	cout << j << " " << nbCommunities << endl;

	assert(j == nbCommunities);

	of.close();

	return true;
}

BFSTree clean_tree(BFSTree t, int k)
{
    
    queue<int> qLast;
    int n = t.n;
    int start;
    int last = n-1; 
   // cout << "taille :" << t.status.size() << endl;
   // cout << "n :" << n << endl;

    for (int i =0 ; i<n ; i++)
    {
        t.status[i] = -1;
    }
  
    while(last >= 0)
    {
       
     while(last >=0 && t.status[last] != -1)   
     {
         last--;
     }
      if(last >= 0) {
     queue<int> q;
     int curr = last;
     qLast.push(last);
     while((t.status[curr]) != 1 && (curr>=0))
     {
         
         q.push(curr);
         t.status[curr] =1;
         curr = t.pere[curr];     
     }
     if (curr==0 && t.status[0] == -1) {q.push(0);  t.status[0] =1;}
     while(!q.empty())
     {
         start = q.front();
         q.pop();     
         elaguer1(k,start,&t);
              
            
          }       
        }
    }
    
    while(!qLast.empty())
    {
       start = qLast.front();
       qLast.pop();
       createExtraLink(2*k+1, start, &t);      
    }
    
   return t; 	   
}



BFSTree clean_treeV2(BFSTree t, int k)
{
    queue<int> qLast;
    int n = t.n;
    t.cycle =0;
    int start;
    int last = n-1; 
   // cout << "taille :" << t.status.size() << endl;
   // cout << "n :" << n << endl;

    for (int i =0 ; i<n ; i++)
    {
        t.status[i] = -1;
    }
  
    while(last >= 0)
    {
       
     while(last >=0 && t.status[last] != -1)   
     {
         last--;
     }
      if(last >= 0) {
     queue<int> q;
     int curr = last;
     qLast.push(last);
     bool intoTheVoid = false;
     while((t.status[curr]) != 1 && (curr>=0))
     {
         if(t.status[curr]==0 && (intoTheVoid  || valid(curr,&t) )) {
             if(intoTheVoid)
             {
                curr = t.pere[curr];
             }
             else
             {
                intoTheVoid = true;
                toClosest(k,curr,&t); 
                q.push(curr);
                t.status[curr] =1;
                curr = t.pere[curr];  
             
             }
         }
         else
         {
         if(intoTheVoid)  {  
         intoTheVoid = false;
         toClosest(k,curr,&t);
         t.cycle++;
         }
         q.push(curr);
         t.status[curr] =1;
         curr = t.pere[curr];  
         }
     }
     if (curr==0 && t.status[0] == -1) {q.push(0);  t.status[0] =1;}
     while(!q.empty())
     {
         start = q.front();
         q.pop();     
         elaguer1(k,start,&t);
              
            
          }       
        }
    }
    
    while(!qLast.empty())
    {
       start = qLast.front();
       qLast.pop();
       createExtraLink(2*k+1, start, &t);    
       //createExtraLink(k+1, start, &t); 
    }
    
   return t; 	   
}

graph createRelique(squelette s)
{
 //   cout << "ISSOU" << endl;
    int hub = -1;
    graph r;
    int curr=0;
    r.n=0;
    r.m=0;
    int tab[s.n];
    
    
    if(s.hub[0]==-1)
    {
         tab[0]=0;
          r.n = r.n +1;
    }
    for (int i = 1; i< s.n ; i++){
        if(s.hub[i]==i ){     
           tab[i]=r.n;
          r.n = r.n +1;
        }     
    }  
   queue<int> qEnd = s.qLast;  
  while(!qEnd.empty())
    {   
      curr = qEnd.front();
       qEnd.pop();
     if(s.hub[curr]==-1){
          tab[curr]=r.n;
          r.n = r.n +1;        
       }
    
  }
    r.g.resize(r.n);
  //  cout << "ISSOU" << endl;

    
    
   queue<int> qLast = s.qLast;  
 while(!qLast.empty())
    {
       curr = qLast.front();
       qLast.pop();
          
        while(s.bfs[curr]!=-1 && curr!=0 ) {
            hub = s.hub[curr];
            if (hub==-1){
                hub =curr;
            }
            while(s.hub[curr]!=-1 && s.bfs[curr]!=-1 && curr!=0 ){
                curr = s.bfs[curr];
            }
            
            while(s.hub[curr]==-1 &&  s.bfs[curr]!=-1 && curr!=0 ){
                curr = s.bfs[curr];
            }         
            if(s.hub[curr]!=-1 || curr==0){
            if(curr==0 && s.hub[curr]==-1){
                r.m++;
                r.g[tab[hub]].push_back(tab[0]);
                r.g[tab[0]].push_back(tab[hub]);
            }      
            else if(hub != s.hub[curr]) // WHY?
            {
                //cout << "hub 1 : " << hub << " hub 2 : " << s.hub[curr] <<endl;
                r.m++;
                r.g[tab[hub]].push_back(tab[s.hub[curr]]);
                r.g[tab[s.hub[curr]]].push_back(tab[hub]); 
            }
            }
       }
    }  
    
    return r;
}

 bool isConnex(graph g)
 {
    int  compt = 1;
        BFSTreeReel t;
	t.n = g.n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(0); 	
        t.pere[0] = 0;
        t.profondeur[0] = 0;	
        
        while(!q.empty()){
		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g.g[p].begin(); it != g.g[p].end(); ++it){
                          if(t.pere[*it] ==-1){
                              compt++;
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;
                                t.fils[p].push_back(*it);
				q.push(*it);

			}
		}
	}
        
        return(compt==g.n);

     
 }


bool isKLamiar(graph g, int k)
{
  int color[g.n];
  for(int i = 0; i < g.n ; color[i++] = -1);
  pair<queue<int>,BFSTreeReel> bfs = stopBFS(make_pair(0,0),&g,color);
  bfs = stopBFS(make_pair(bfs.first.front(),bfs.first.front()),&g,color);
  cout << "size of path : " << bfs.first.size() << endl;
   int close[g.n];
   int p;
   for(int i = 0; i < g.n ; close[i++] = -1);
  queue<int> q = bfs.first;
  
  while(!q.empty()){
        
        
    p = q.front();
    q.pop();
    colorPath(k, p, &g, close);
	}

  
  
  for (int i=0; i < g.n ; i++)
  {
     if(close[i]== -1) 
         return false;
  }
  return true;
}

int findFirstHub(graph g, int k, int r)
{
  int color[g.n];
  for(int i = 0; i < g.n ; color[i++] = -1);
  pair<queue<int>,BFSTreeReel> bfs = stopBFS(make_pair(0,0),&g,color);
 /* int p = hubMiddle(bfs.first,0,&g,k,color); 
  if(p!=-1)
      return p;
   */
  return bfs.first.front();
}
/*
 Input : Graphe g, rayon r, lailinaritÃ© k, sommet de dÃ©part de l'algo s
 * Output : Liste de hubs
 */

queue<int> findhubs(graph g, int k, int r, int s)
{
int color[g.n];

for (int i = 0 ; i<g.n ; i++){
    color[i]=-1;
   
}
queue<int> A ;  
A.push(s);
colorPartialBFS(r,s,s,&g,color);
pair<int,int> curr = uncolored(r+1,A,&g,color); 
while(curr.first != -1 )  
{
   // cout << "d " << curr.second << endl;

 // cout << "hub courr " << curr.first << endl;
    //cout << "curr hub = " << curr.first << " curr depart = " << curr.second << endl;
 pair<queue<int>,BFSTreeReel> bfs = stopBFS(curr,&g,color);
 
// cout << "bfs done"  << endl;
 if (bfs.first.size() <= r) //2*r+4*k+2 SI L GRAND
 {
  // cout << "small bfs of size " << bfs.first.size() << endl;
  colorBFS(bfs.second,curr.second,color);  
 }
 else{
     int p = hubMiddle(bfs.first,curr.first,&g,k,color);
    // cout << "test hub middle done" << endl;
     if(p!=-1){
        // cout << "hub mid " << p << endl;
         A.push(p);
         colorPartialBFS(r,p,p,&g,color);
 }
     else if( closeToA(bfs.first.front(),curr.first,k+1,&g,color) && bfs.first.size()>1){ //2k SI EXACT ET L GRAND
      //  cout << "close to a" << endl;
         queue<int> path = bfs.first;
         for(int i= 0; i<path.size()-1;i++){
             path.pop();
         }
         A.push(path.front());
         colorPartialBFS(r,path.front(),path.front(),&g,color);
         colorLamCycle(k,bfs.first,path.front(),curr.first,color, &g);
     }     
     else if( !adjacentToA(bfs.first.front(),&g,color))
     {
      //   cout << "hub dead end "<< bfs.first.front() << endl;
         A.push(bfs.first.front());
         colorPartialBFS(r,bfs.first.front(),bfs.first.front(),&g,color);
     }
     else{
  //   cout << "laminar " << bfs.first.front() << endl;
    /*
         queue<int> l = bfs.first;
        while(!l.empty())    {
            cout << l.front() << endl;
            l.pop();
            
        }
         */
      int hub = 0; 
      for (std::vector<int>::iterator it = g.g[bfs.first.front()].begin(); it != g.g[bfs.first.front()].end();it++){
      if(color[*it] >= 0){
      hub = color[*it];
      }
     }
       colorLamCycle(k,bfs.first,hub,curr.first,color, &g);  
     }
    
}
 curr = uncolored(r+1,A,&g,color); 
    
}

/*
for(int i =0 ; i<g.n ; i++){
    if(color[i]==-1)
    {   
        int curr = i;
         int close[g.n];   
         for(int j = 0; j < g.n ; close[j++] = -1);
         queue<int> q;
         q.push(curr);
         close[curr]=0;
         bool stop = false;
         while(!q.empty() && stop){
         curr=q.front();
         if(color[curr]>=0)
             stop = true;
         for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
                    if(close[*it]==-1 && close[curr]<r+1){
                        close[*it]=close[curr]+1;
                        q.push(*it);
                    }
               }
            q.pop();
            }
        
         if(!stop){
     //   cout << "hub lam decomposition could not be computed" << endl;
        queue<int> emptyQ;
        return emptyQ;     
         }
    } 
}
*/
// Hub intersecting

if(r!=0){
for(int i =0 ; i<g.n ; i++){
  if(color[i]>=0)
    {  
       for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end();it++){
           if(color[*it]>=0 && color[*it]!=color[i]){
        queue<int> emptyQ;
        return emptyQ;  
           }
            }      
  }   
}
}

 
return A;
}
/*
 Input : Graphe g, rayon r, lailinaritÃ© k, liste de hubs A
 * Output : Liste de chemins reliant les hubs
 */

queue<queue<int>> findlaminars(graph g, int k, int r, queue<int> A) {
    queue<queue<int>> Q;
    bool b = true;
    bool deleted[g.n];
    bool deletable[g.n];

    for (int i = 0; i < g.n; deletable[i++] = true);
    for (int i = 0; i < g.n; deleted[i++] = false);
    queue<int> oldpath;
    queue<int> newpath;
    queue<int> copyA = A;
    while (!copyA.empty()) {
        deletePartialBFS(r, copyA.front(), &g, deletable);
        copyA.pop();
    }
    copyA = A;
    int olda=-2;
    int fin=0;
    int a = undeleted(r + 1, A, &g, deleted,deletable);
    while (a != -1) {
      //  cout << " a " << a <<endl;
        queue<int> path = computePath(a, A, &g, deleted,deletable);
        
        queue<int> copyP = path;       
        //bool bo =false;
       /* while(!copyP.empty())
        {
            if(deletable[copyP.front()])
                bo=true;
            copyP.pop();
        }*/
         
       /* if(!bo){
            int j=0;
        copyP = path;
              while(!copyP.empty())
        {
            if(j==path.size()/2)
                deleted[copyP.front()]=true;
          j++;
            copyP.pop();
        }
        }         */           
        if(!path.empty()){       
        b = true;
        newpath = path;
        if (!oldpath.empty() && !newpath.empty()) {
            while (!newpath.empty() && !oldpath.empty() && b) {
                if (newpath.front() != oldpath.front())
                    b = false;
                newpath.pop();
                oldpath.pop();

            }
        } else {
            b = false;
        }


        if (!b) {
            if (!path.empty()) {
                Q.push(path);
                //cout << " debut path : " << path.front() << " fin path : " << path.back() << endl;
            }
            if(path.size()!=2)
            deletePath(k, path, &g, deletable, deleted);
            else
            { // On supprime l'edge si le path est simplement entre deux hubs
          for (std::vector<int>::iterator it = g.g[path.front()].begin(); it != g.g[path.front()].end();){
              if(*it==path.back())
                  it = g.g[path.front()].erase(it);       
               else
              it++;
          }
           for (std::vector<int>::iterator it = g.g[path.back()].begin(); it != g.g[path.back()].end();){
              if(*it==path.front())
                  it = g.g[path.back()].erase(it);       
               else
              it++;
          }
            }

            oldpath = path;
            a = undeleted(r + 1, A, &g, deleted,deletable);
        } else {
            a = -1;
        }
        }
        else
          a = undeleted(r + 1, A, &g, deleted,deletable);  
       
        if(olda==a)
            fin++;
        else
        {
            olda=a;
            fin=0;
        }
        if(fin==20)
            a=-1;
    }
    
    queue<queue<int>> copyQ = Q;
    queue<int> bigQ;
    int color[g.n];
    int isHub[g.n];
    int dist[g.n];
    for (int i = 0; i < g.n; color[i++] = -1);
    for (int i = 0; i < g.n; dist[i++] = -1);
    for (int i = 0; i < g.n; isHub[i++] = -1);


    int intit = 0;

    while (!copyQ.empty()) {
        queue<int> tempQ = copyQ.front();
        while (!tempQ.empty()) {
            bigQ.push(tempQ.front());
            color[tempQ.front()] = intit;
            dist[tempQ.front()] = 0;
            tempQ.pop();
        }
        intit++;
        copyQ.pop();
    }

    while (!copyA.empty()) {
        colorPartialBFS(r, 0, copyA.front(), &g, isHub);
        copyA.pop();
    }

    while (!bigQ.empty()) {
        int curr = bigQ.front();
        for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end(); it++) {
            if (dist[*it] == -1 && dist[curr] < k) {
                dist[*it] = dist[curr] + 1;
                if (color[*it] == -1)
                    color[*it] = color[curr];
                bigQ.push(*it);
            }
        }
        bigQ.pop();
    }
   

    
    
    for (int i = 0; i < g.n; i++) {
        /*
        if (color[i] == -1 && isHub[i] == -1) {
          //  cout << "unco "<< i << endl;
            queue<queue<int>> emptyQ;
            return emptyQ;
        }
        */
        for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end(); it++) {
            if (isHub[i] == -1 && isHub[*it] == -1 && color[i] != color[*it]) {
                // Deux laminaires se touchent.
                queue<queue<int>> emptyQ;
                return emptyQ;
            }
        }
       
    }
    
    return Q;
}

/*
 Input : Un chemin P, un graph contenant des sommets supprimables, une entier "profondeur".
 * Output : Les sommets supprimables Ã  distance "profondeur" du chemin P sont supprimÃ©s.
 */

void deletePath(int profondeur, queue<int> P, graph * g, bool * deletable,bool *deleted ){
 
    
   int close[g->n];
    int p;
    for(int i = 0; i < g->n ; close[i++] = -1);
    queue<int> q = P;
    
     while(!q.empty()){     
    close[q.front()]=0;
    q.pop();
	}
    
    q =P;
    while(!q.empty()){     
    p = q.front();
    if (deletable[p])
       deleted[p]=true;
    for (std::vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end();it++){
        // if(close[*it]==-1 && close[p]<profondeur)
        if(close[*it]==-1 && close[p]<profondeur && deletable[*it] && !deleted[*it])
        {       
            close[*it]=close[p]+1;
            q.push(*it);
        }      
    }   
    q.pop();
	}
    
    
    return ;
}

/*
Input : Un sommet "curr", un graph contenant des sommets supprimables, une entier "profondeur"
 * Output : Supprime le sommet "curr" et tout les sommets Ã  distance "profondeur" de "curr". 
 * L'action prÃ©cÃ©dente s'effectie uniquement sur les sommets marquÃ©s comme supprimable.
 */

void deletePathK(int profondeur,int curr,graph * g,bool * deletable, bool * deleted,int * close ){
    close[curr]=profondeur;
    if (deletable[curr])
        deleted[curr]=true;
    
    if(profondeur==0)
        return;
        
     for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
      if(close[*it]<profondeur){
         if(close[*it]!= -1 || !deleted[*it])
     deletePathK(profondeur-1,*it,g,deletable,deleted,close); 
     }
     }
return;
    
}

/*
 Input : Un sommet de dÃ©part, un ensemble de sommet A
 * Output : Le plus court chemin entre le sommet de dÃ©part et un sommet de A.
 * Note : Si aucun chemin n'existe reliant le sommet de dÃ©part Ã  A, une erreur est renvoyÃ©e.
 */
queue<int> computePath(int start,queue<int> A,graph *g,bool* deleted, bool * deletable)
 {
   queue<int> P;
   queue<int> copyA = A;
   int last = -1;
   BFSTreeReel t;
   bool tabA[g->n];
   for(int i = 0; i < g->n ; tabA[i++] = false);
   while(!copyA.empty())
   {
       int a = copyA.front();
       copyA.pop();
       tabA[a] = true;     
   }
   tabA[start] = false;
    t.n = g->n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(start); 	
        t.pere[start] = start;
        t.profondeur[start] = 0;	
        bool stop = false;

        while(!q.empty() && !stop){
		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end() && !stop; ++it){
                    if(tabA[*it])
                    {
                        stop =true;
                        last = *it;
                        t.pere[last] = p;
                        /*
                        while(t.pere[last] != last)
                        { 
                            last = t.pere[last];
                            if(deletable[last])
                                stop = true;
                        }     */                   
                        
                    }
                    else if(!deleted[*it] && t.pere[*it] ==-1)
                    {      
                         t.pere[*it] = p;
                         t.profondeur[*it]=t.profondeur[p]+1;                             
                         t.fils[p].push_back(*it);                     
                         q.push(*it);
                    }
		}
	}
        if(!stop)
        {
          //  cout << "no stop"<<endl;
            deleteBFS(t,start,deleted);
            return P;
        }
        P.push(last);
        
        while(t.pere[last] != last)
        { 
           // cout << last << " ";
            last = t.pere[last];
            P.push(last);          
        }
      //  cout << endl;
       // cout << P.front() << " et " << P.back() << endl; ;
    return P;
 }

void  deleteBFS(BFSTreeReel t,int start,bool* deleted){
    
     queue<int> q;
     q.push(start); 	
     while(!q.empty()){
        int p = q.front();
	q.pop();
	deleted[p]=true; 
		for(deque<int>::iterator it = t.fils[p].begin(); it != t.fils[p].end(); ++it){                                                                             
				q.push(*it);                   
		}
			
	}
    
}

/*
 Input : Un entier "profondeur", un ensemble de sommets A
 * Output : Un sommet Ã  distance "pronfondeur" d'un sommet de A, -1 si un tel sommet n'existe pas.
 */

int undeleted(int profondeur,queue<int> A,graph *g,bool* deleted, bool* deletable)
{
    int hubCurr;
  queue<int> q = A;
  int curr;
  int dist[g->n];
  for(int i = 0; i < g->n ; dist[i++] = -1);
  
    while(!q.empty())
    {
        for(int i = 0; i < g->n ; dist[i++] = -1);
        queue<int> bfs;
        bfs.push(q.front());
        hubCurr=q.front();
        dist[q.front()]=0;
        while(!bfs.empty())  {
            curr = bfs.front();
            if(dist[curr]==profondeur)
            {
                 if(!deleted[curr] && deletable[curr])
                   return hubCurr;
            }
            else{
            for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
                if(dist[*it]==-1){
                   dist[*it] = dist[curr]+1;
                bfs.push(*it);               
                }
                 }
            }
                    bfs.pop();        
    }
        q.pop();
    }
  return -1;
    
}

/*
 Input : Un entier "profondeur", un sommet "curr"
 * Output : Un sommet Ã  distance "pronfondeur" de "curr", -1 si un tel sommet n'existe pas.
 */

bool undeletedClose(int profondeur,int curr,graph *g,bool* deleted, int* close){
    close[curr]=profondeur;
  if (profondeur ==0)      
   return !deleted[curr];
  
  
  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
      if(close[*it]< profondeur){
     if(undeletedClose( profondeur-1,*it, g, deleted,close))
         return true;
     }
  }
    return false;    
}


int trueUndeleted(int profondeur,queue<int> A,graph *g,bool* deleted)
{
  queue<int> q = A;
  int close[g->n];  
    while(!q.empty())
    {
        int s=q.front();
        q.pop();
       for(int i = 0; i < g->n ; close[i++] = -1);
       if(undeletedTrueClose(profondeur,s,g,deleted,close))
           return s; 
    }
  return -1;
}

/*
 Input : Un entier "profondeur", un sommet "curr"
 * Output : Un sommet Ã  distance "pronfondeur" de "curr", -1 si un tel sommet n'existe pas.
 */

bool undeletedTrueClose(int profondeur,int curr,graph *g,bool* deleted, int* close){
    close[curr]=profondeur;
  if (profondeur ==0)      
   return deleted[curr];
  
  
  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
      if(close[*it]< profondeur){
     if(undeletedTrueClose( profondeur-1,*it, g, deleted,close))
         return true;
     }
  }
    return false;    
}





 void deletePartialBFS(int profondeur, int curr, graph *g, bool* deletable){
     int close[g->n];   
  for(int i = 0; i < g->n ; close[i++] = -1);
  queue<int> q;
  q.push(curr);
  close[curr]=0;
  
  while(!q.empty()){
  curr=q.front();
  deletable[curr]=false;
  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
          if(close[*it]==-1 && close[curr]<profondeur){
              close[*it]=close[curr]+1;
              q.push(*it);
          }
     }
  q.pop();
  }
    return;
     
     
    
   
}

  void deleteTruePartialBFS(int profondeur, int curr, graph *g, bool* deletable, int* close){
  close[curr]=profondeur;
  deletable[curr]=true;
  if (profondeur == 0)
      return;

  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
            if(close[*it]<profondeur){
     deleteTruePartialBFS(profondeur-1,*it,g,deletable,close);
            }
     }
   
    return;
}
 /*
  * Input : Un sommet
  * Output : True si le sommet est adjacent Ã  un sommet coloriÃ© par un hub, False sinon
  */

bool adjacentToA(int f,graph * g,int* tabColor){
     for (std::vector<int>::iterator it = g->g[f].begin(); it != g->g[f].end();it++){
      if(tabColor[*it] >= 0)
      return true;    
     }
   return false; 
}

/*
 * Input : Deux hubs, un chemin et une profondeur.
 * Output : Les sommets Ã  distance au plus profondeur du chemin ont la couleur -2.
 * L'opÃ©ration dÃ©crite en output s'effectue dans le graphe privÃ© des sommets Ã  distance moins de r des hubs donnÃ©s en input.
 */
 void colorLamCycle(int profondeur, queue<int> P, int hub1,int hub2,int * tabColor, graph * g){
    queue<int> q = P;
     int close[g->n];   
  for(int i = 0; i < g->n ; close[i++] = -1);
  while(!q.empty())
  {
      close[q.front()]=0;
      q.pop();

  }
     q=P;
    
    while(!q.empty()){      
    int p = q.front();
    if(tabColor[p]<0)
        tabColor[p]=-2;
      for (std::vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end();it++){
          if(close[*it] ==-1 && close[p]<profondeur  && tabColor[*it ] != hub1 && tabColor[*it ] != hub2)
          {
              close[*it]=close[p]+1;
              q.push(*it);
          }        
      }
      q.pop();
    }      
    return ;
}
 
 /*
 * Input : Deux hubs, un sommet "curr" et une profondeur.
 * Output : Les sommets Ã  distance au plus profondeur du sommet "curr" ont la couleur -2.
 * L'opÃ©ration dÃ©crite en output s'effectue dans le graphe privÃ© des sommets Ã  distance moins de r des hubs donnÃ©s en input.
 */
 
void colorLamCycleK(int profondeur,int curr,int hub1,int hub2,int * tabColor,bool * treated, graph * g){
     if(profondeur==0)
      return;
    treated[curr]=true;
    if(tabColor[curr] <0)
        tabColor[curr]=-2;
   
        
     for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
    
      if(!treated[*it] && tabColor[*it ] != hub1 && tabColor[*it ] != hub2)
        colorLamCycleK(profondeur-1,*it,hub1,hub2,tabColor,treated,g);     
     }
return;
    
}

/*
 Input : Un sommet "curr", une couleur et une profondeur
 * Output : True si il existe un sommet de la couleur donnÃ©e Ã  distance moins de "profondeur" du sommet "curr"
 */

bool closeToA(int curr,int c,int profondeur ,graph* g, int* tabColor){
    
    
  int close[g->n];   
  for(int i = 0; i < g->n ; close[i++] = -1);
  queue<int> q;
  q.push(curr);
  close[curr]=0;
  
  while(!q.empty()){
  curr=q.front();
     if(tabColor[curr]==c)
      return true;
    for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
          if(close[*it]==-1 && close[curr]<profondeur){
              close[*it]=close[curr]+1;
              q.push(*it);
          }
     }
  q.pop();
  }
    return false;
     
}


int hubMiddle(queue<int> P, int color, graph * g,int k, int* tabColor)
{
    
    int close[g->n];
    int close2[g->n];
    int p;
    for(int i = 0; i < g->n ; close[i++] = -1);
    for(int i = 0; i < g->n ; close2[i++] = -1);
    queue<int> q = P;
    queue<int> q2;

   while(!q.empty()){     
    close[q.front()]=0;
    q.pop();
	}
    
    q =P;
    while(!q.empty()){     
    p = q.front();
    for (std::vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end();it++){
        if(close[*it]==-1 && close[p]<k)
        {
            close[*it]=close[p]+1;
            q.push(*it);
        }      
    }   
    q.pop();
	}
    
    q = P;
    
    int profondeur = 0;
    while(!q.empty()){
    if((profondeur>3*k) && (profondeur <= P.size()-3*k))
    {
        q2.push(q.front());
    }
    profondeur++;
     q.pop();
    }
    
    /*
   while(!q.empty()){     
    close2[q.front()]=0;
    q.pop();
	}*/
    
    while(!q2.empty())
    {
        
       q.push(q2.front());
        for(int i = 0; i < g->n ; close2[i++] = -1);
        close2[q2.front()]=0;
        while(!q.empty())
        {
         p=   q.front();
        for (std::vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end();it++){
        if(close[*it]==-1 && tabColor[*it]!=color)     
        {
            //cout << "le sommet qui fait trigger : " << *it << "avec k " << k << " et q2.front " << q2.front() << " et close2 " <<
            // close2[p] << endl;
            return q2.front();
        }
        if(close2[*it]==-1 && close2[p]<k)
        {
            close2[*it]=close2[p]+1;
            q.push(*it);
        } 
        
        }
         q.pop();
        
    }   
    q2.pop();
        
    }
    return -1;
}

/*
 Input : Une chmin, une couleur "color", un entier k
 * Output : True si il existe un sommet de couleur diffÃ©rente de "color" Ã  distance k+1 du chemin et Ã  distance k+1
 * d'un sommet Ã©loignÃ© de 3k d'une des extrÃ¨mitÃ©s du chemin.
 
int hubMiddle(queue<int> P, int color, graph * g,int k, int* tabColor)
{
    
    int close[g->n];
    int close2[g->n];
    int p;
    for(int i = 0; i < g->n ; close[i++] = -1);
    queue<int> q = P;
    
    
    while(!q.empty()){
        
        
    p = q.front();
    q.pop();
    colorPath(k, p, g, close);
	}
    
    q = P;
    int profondeur = 0;
    for(int i = 0; i < g->n ; close2[i++] = -1);
     while(!q.empty()){
    int p = q.front();
    q.pop();
    if((profondeur>3*k) && (profondeur <= P.size()-3*k))
    {
        if(searchCandidate(k+1,color,p,g,close,close2,tabColor))
        return p;
    }
    profondeur++;
    }
    return -1;
}
  */

/*
 * Input : Une couleur "color", un sommet "curr", deux ensembles de couleurs
 Output : Renvoit true si il existe un sommet Ã  distance k+1 de "curr" d'une couleur diffÃ©rente de "color"
 * et tel que sa couleur dans "close" soit "-1".
 * Les sommets Ã  distance moins de k du chemin de "hubinthemidlle" ayant une couleur dans "close" de 1.
 */

bool searchCandidate(int k,int color,int curr,graph * g,int* close,int* close2, int* tabColor){
    close2[curr]=k;
    if(k==0)
    {
        return (tabColor[curr]!=color && close[curr]==-1);      
    }
    
     for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
         
      if(close2[*it] < k){
      if(searchCandidate( k-1,color,*it, g,close,close2,tabColor))  
         return true;
     }
  }
    return false;
    
}

/*Input : Un arbre BFS
 * Output : Les sommets de l'arbre non coloriÃ©s ont maintenant la couleur "-2" (Ã©quivalent de "lam")
 */

void  colorBFS(BFSTreeReel t,int start,int* tabColor){
    
     queue<int> q;
     q.push(start); 	
     while(!q.empty()){
        int p = q.front();
	q.pop();
        if(tabColor[p]== -1)
	tabColor[p]=-2; 
        for(deque<int>::iterator it = t.fils[p].begin(); it != t.fils[p].end(); ++it){                                                                             
				q.push(*it);                   
	}
			
	}
    
}

/*
 Input : Une couleur (premier Ã©lÃ¨ment de la paire), un sommet de dÃ©part (second Ã©lÃ¨ment) de la paire, un graph coloriÃ©.
 * Output : Un plus court chemin reliant le sommet le sommet de dÃ©part Ã  un sommet de couleur diffÃ©rente 
 * de celle donnÃ©e en entrÃ©e. Si cela n'existe pas, le plus long plus court chemin partant du sommet de dÃ©part.
 * Le BFS ayant servi au calcul de ce chemin est Ã©gallement retournÃ©.
 */


 pair<queue<int>,BFSTreeReel> stopBFS(pair<int,int> curr,graph *g,int* tabColor)
 {
    int color = curr.first;
    int start = curr.second;
    queue<int> P;
   int profondeur = -1;
   int last = start;
   BFSTreeReel t;
	t.n = g->n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(start); 	
        t.pere[start] = start;
        t.profondeur[start] = 0;	
        bool stop = false;
        
        
        while(!q.empty() && !stop){
		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end() && !stop; ++it){
                   // if(tabColor[*it]!= color && tabColor[*it] != -1 )
                    if(tabColor[*it]!= color && tabColor[*it] != -1 && tabColor[*it] != -2 )
                    {
                        stop = true;
                        last = p;
                    }
                    else if(tabColor[*it]==-1)
                    {                 
                                if(t.pere[*it] ==-1 && tabColor[*it]!= color){
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;                             
                                t.fils[p].push_back(*it);
                                if(t.profondeur[*it]>profondeur)
                                {
                                    profondeur = t.profondeur[*it];
                                    last = *it;
                                }
                                
				q.push(*it);

			}
                    }
		}
	}
        
        P.push(last);
        while(t.pere[last] != last)
        {
            last = t.pere[last];
            P.push(last);          
        }

    return make_pair(P,t);
 }
 
 /*
  Input : Une profondeur, une couleur "color", un sommet de dÃ©part.
  * Output : Tout les sommets Ã  distance au plus "pronfondeur" du sommet de dÃ©part ont la couleur "color".
  */
 
 void colorPartialBFS(int profondeur, int color, int curr, graph *g, int* tabCol){
     
  int close[g->n];   
  for(int i = 0; i < g->n ; close[i++] = -1);
  queue<int> q;
  q.push(curr);
  close[curr]=0;
  
  while(!q.empty()){
  curr=q.front();
  tabCol[curr]=color;
  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
          if(close[*it]==-1 && close[curr]<profondeur){
              close[*it]=close[curr]+1;
              q.push(*it);
          }
     }
  q.pop();
  }
    return;
}

 void colorPath(int profondeur, int curr, graph *g, int* close){
  close[curr]=profondeur;
  if (profondeur !=0){

  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
            if(close[*it] < profondeur){
     colorPath(profondeur-1,*it,g,close);
            }
     }
   }
    return;
}
    
    
/*
 * Input : Un ensemble de sommets (correspondants aux hubs), une profondeur.
Output :  si il existe un sommet non coloriÃ© Ã  distance profondeur+1 d'un hub, une paire avec :
 * Premier Ã©lÃ¨ment : le hub
 * Second Ã©lÃ¨ment : vertex non coloriÃ© Ã  distance pronfondeur+1 du hub
 */

 
 pair<int,int> uncolored(int profondeur,queue<int> A,graph *g,int* tabCol)
{
   int currHub;
  queue<int> q = A;
  int curr;
  int dist[g->n];
  
    while(!q.empty())
    {
         for(int i = 0; i < g->n ; dist[i++] = -1);
        queue<int> bfs;
        bfs.push(q.front());
        dist[q.front()]=0;
        currHub = q.front();
        while(!bfs.empty())  {
            curr = bfs.front();
            if(dist[curr]==profondeur)
            {
                 if(tabCol[curr]==-1)
                   return make_pair(currHub,curr);
            }
            else{
            for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
                if(dist[*it]==-1){
                   dist[*it] = dist[curr]+1;
                bfs.push(*it);               
                }
                 }
            }
                    bfs.pop();        
    }
        q.pop();
    }
  return make_pair(-1,-1);
}
 /*
pair<int,int> uncolored(int profondeur,queue<int> A,graph *g,int* tabCol)
{

  queue<int> q = A;

 int close[g->n];  
 

  for(int i = 0; i < g->n ; close[i++] = -1);
  

    while(!q.empty())
    {
        int s=q.front();
        q.pop();
        int d = uncoloredClose(profondeur,s,g,tabCol,close);
       if(d!= -1)
           return make_pair(s,d);                
       for(int i = 0; i < g->n ; close[i++] = -1);
    }
  return make_pair(-1,-1);
}
   */
  
/*
 * Input : Une profondeur et un sommet "curr"
 * Output : Un sommet non coloriÃ© Ã  distance profondeur+1 de "curr",
 * -1 si un tel sommet n'existe pas
 */
int uncoloredClose(int profondeur,int curr,graph *g,int* tabCol,int* close){
    close[curr]=profondeur;
  if (profondeur ==0){ 
      if(tabCol[curr]==-1)
          return curr;
      return -1;
  }

  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
      if(close[*it]<profondeur){
     int d = uncoloredClose( profondeur-1,*it, g, tabCol,close);
     if(d !=-1)
         return d;
      }   
  }
    return -1;
    
}






squelette algoReel(graph g, int k, int r)
{
    BFSTreeReel t = BFS_tree_reel(g,0);     
    squelette s;
    s.n=g.n;
    s.m =g.m;
    s.g = g.g;
       
    int n = s.n;
    int color = 0;
    int start;
    int last =-1;

    for (int i =0 ; i<n ; i++)
    {
      s.closest.push_back(n+1);
      s.father.push_back(-1);
      s.color.push_back(-1);
      s.hub.push_back(-1);   
      s.bfs.push_back(-1);
    }
    
    
    
    
    
    
    last = deepestUncolored(&s,&t);
    while(last>=0)
    {
    color++;
   
      if(last >= 0) {
     queue<int> q;
     int curr = last;
     s.qLast.push(last);
     while(s.color[curr] == -1 && (curr>=0))
     {
         q.push(curr);
         s.color[curr] = color;
         s.closest[curr] =0;
         s.father[curr] = curr;
       //  if(s.color[t.pere[curr]]==-1){
         s.bfs[curr] = t.pere[curr]; 
       //  }
         curr = t.pere[curr];     
     }
     
     if (curr==0 && s.color[0] == -1) {q.push(0);  s.color[0] = color;}
     
     while(!q.empty())
     {
         start = q.front();
         q.pop();     
         elaguerReel(k,0,color,start,start,&s);             
          }     
     
    }
        last = deepestUncolored(&s,&t);
    }
    
    
    
    

    for (int i = 0; i< s.n ; i++){
          for (std::vector<int>::iterator it = s.g[i].begin(); it != s.g[i].end();it++){
              if(s.color[i]!=s.color[*it] ) {
               //   if(!hubClose(r,i,&s)){
                  if(s.hub[i]==-1){
                      s.hub[i]=i;
                     fillHub(i,k,r,&s);
                  }
                // }                
              }                            
          }
              
    }
    
    
    
    
    for (int i = 0; i< s.n ; i++){
          for (std::vector<int>::iterator it = s.g[i].begin(); it != s.g[i].end();it++){
              if(s.hub[i]!=-1 && s.hub[*it] !=-1) {
                  if(s.hub[i] != s.hub[*it]) {
                     fusionHub(s.hub[i],s.hub[*it],&s);                
                  }                
              }                            
          }
              
    }
  
 
   
    
   return s; 	   
}

void fusionHub(int h,int l,squelette *s ){
    for (int i = 0; i< s->n ; i++){
        if(s->hub[i]==l){
           s->hub[i]=h;
        }
    }  
}            



void fillHub(int h,int k,int r,squelette *s)
{
        BFSTreeReel t;
	t.n = s->n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(h); 	
        t.pere[h] = h;
        t.profondeur[h] = 0;	
        
        while(!q.empty()){
		int p = q.front();
		q.pop();
              //  if(s->closest[p]==0 && s->hub[p]!=p){ A VOIR
                
                    s->hub[p]=h;
             //   }
              //  if((t.profondeur[p]<(r+k)) || ((t.profondeur[p]<(r+2*k)) && (s->closest[p]==0))){
                if(t.profondeur[p]< r){
		for(vector<int>::iterator it = s->g[p].begin(); it != s->g[p].end(); ++it){
                          if(t.pere[*it] ==-1){
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;
                                t.fils[p].push_back(*it);
				q.push(*it);

			}
		}
                }
	}
 for (int i=0;i<s->n;i++)
 {
     if(s->hub[s->father[i]]==h && s->hub[i]!=i)
     {
         s->hub[i]=h;
     }
 }
    
}


int deepestUncolored(squelette *s, BFSTreeReel *t){
    int profondeur = -1;
    int resultat = -1;
    for (int i=0;i<s->n;i++)
    {
        if (s->color[i] == -1){
            if(t->profondeur[i] > profondeur){
                resultat = i;
                profondeur = t->profondeur[i];
            }
        }
    }
    return resultat;
}


void elaguerReel(int k, int profondeur, int color, int curr, int start, squelette *s){
    if (k ==0){
       if (profondeur < s->closest[curr]){
       s->color[curr]=color;
       s->closest[curr]=profondeur;
       s->father[curr]=start;
       }
            return ;
    }
   
  for (std::vector<int>::iterator it = s->g[curr].begin(); it != s->g[curr].end();it++){
      if (profondeur < s->closest[*it]){
       s->color[*it]=color;
       s->closest[*it]=profondeur;
       s->father[*it]=start;
       } 
     elaguerReel(k-1,profondeur+1,color,*it,start,s);
     }
    
}


bool hubClose(int r, int i ,squelette * s){

        BFSTreeReel t;
	t.n = s->n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(i); 	
        t.pere[i] = i;
        t.profondeur[i] = 0;	
        
        while(!q.empty()){
		int p = q.front();
		q.pop();
                if(s->hub[p]==p)
                    return true;
                if(t.profondeur[p]<r){
		for(vector<int>::iterator it = s->g[p].begin(); it != s->g[p].end(); ++it){
                          if(t.pere[*it] ==-1){
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;
                                t.fils[p].push_back(*it);
				q.push(*it);

			}
		}
                }
	}
   
        return false;
 }


bool valid(int source, BFSTree * t)
{
    int curr = source;
         while((t->status[curr] != 1) && (t->pere[curr] != curr))
         {
             if (t->status[curr] == -1)  
                 return true;
             curr = t->pere[curr];
         }
    return false;
    
    
}

void toClosest(int k, int start, BFSTree * t)
{
         int n = t->n;
        int x,a;
        int par[n];
  	for(int i=0; i<n; ++i) {
            par[i] = -1;
        }

	
	queue<int> q;
	q.push(start);
	par[start] = start;
	while(!q.empty()){
		x = q.front();
                if(t->status[x] == 1)
                {
                    if (!son(k,x,start,t))
                    {
                        t->extraLink.push_back(make_pair(start,x));
                        t->voisins[start].push_back(make_pair(x,1));
                        t->voisins[x].push_back(make_pair(start,1));
                        
                    return ;
                    }
                }
		q.pop();
		for(int j = 0; j < t->voisins[x].size(); j++){
                    a = t->voisins[x][j].first;
			if(par[a] == -1){
				par[a] = x;
				q.push(a);
                        }
		}
		
	}
    
}

bool son(int k,int start ,int goal ,BFSTree * t)
{
    int i = 0;
    int curr = start;
    while(t->pere[curr] != curr && i<=k)
    {
        if (curr == goal)
            return true;
        else
            curr = t->pere[curr];
        
        
    }
    return false;
}

bool createExtraLink(int k, int start, BFSTree * t)
{
        int n = t->n;
        int x,a;
        int par[n];
        int select[n];
  	for(int i=0; i<n; ++i) {
            par[i] = -1;
            select[i] = -1;
        }
	vector<int> dis(n,0);

	
	queue<int> q;
	q.push(start);
	par[start] = start;
	while(!q.empty()){
		x = q.front();
                if(t->status[x] == 1)
                    select[x]=1;
		q.pop();
                if(dis[x]<=k){
		for(int j = 0; j < t->voisins[x].size(); j++){
                    a = t->voisins[x][j].first;
			if(par[a] == -1){
				par[a] = x;
				dis[a] = dis[x] + 1;
				q.push(a);
                        }
		}
		}
	}
        for(int i=0; i<n; ++i) {
            par[i] = -1;
            dis[i] = 0;
        }
        q.push(start);
	par[start] = start;
	while(!q.empty()){
		x = q.front();
                if(t->status[x] == 1)
                    select[x]=-1;
		q.pop();
                if(dis[x]<=k*2){
		for(int j = 0; j < t->voisins[x].size(); j++){
                    a = t->voisins[x][j].first;
			if((par[a] == -1) && (t->voisins[x][j].second == 1)){
				par[a] = x;
				dis[a] = dis[x] + 1;
				q.push(a);
                        }
		}
		}
	}
        
     for(int i = 1; i<n ; i++)
     {
         if(select[i]==1){
           t->extraLink.push_back(make_pair(start,i));
           t->voisins[start].push_back(make_pair(i,1));
           t->voisins[i].push_back(make_pair(start,1));
           t->cycle ++;
           return true;
         }
     }
	
   return false; 
}


void elaguer1(int k,  int start, BFSTree *t){
        int n = t->n;
        int x,a;
        int par[n];
  	for(int i=0; i<n; ++i) {
            par[i] = -1;
        }
	vector<int> dis(n,0);

	
	queue<int> q;
	q.push(start);
	par[start] = start;
	while(!q.empty()){
		x = q.front();             
		q.pop();
                if(dis[x]<k){
		for(int j = 0; j < t->voisins[x].size(); j++){
                    a = t->voisins[x][j].first;
			if(par[a] == -1){
                            if(t->status[a]==-1)
                                t->status[a]=0;
				par[a] = x;
				dis[a] = dis[x] + 1;
				q.push(a);
                        }
		}
	}
	
    }
   
    
}


void elaguer(int k,  int start, BFSTree *t){
    if (k ==0){
        
       t->status[start]=0;
            return ;
    }
    bool b = true;
   
  for (std::deque<int>::iterator it = t->fils[start].begin(); it != t->fils[start].end();it++){
     elaguer(k-1,*it,t);
     if(t->status[*it]!=0)
         b =false;
     }
  
    if(b){
       t->status[start]=0;
    }
    
}

void create_graph_from_tree(graph & g, BFSTree t)
{
    int n = 0;
    int j =0;
    int tab[t.n];
    for (int i = 0; i< t.n; i++)
    {       
    //    cout << "je suis : " << i << "pere :" << t.pere[i] << " statut " << t.status[i] << "statut pere :" << t.status[t.pere[i]]<< endl;
        tab[i] = -1;
        if( t.status[i] == 1)
            n++;
    }
    g.n = n;
    g.g.resize(g.n);

    for(int i =0; i< t.n; ++i){
        if(t.status[i] == 1 )
        {
        tab[i]=j;
        
        if(t.status[t.pere[i]]==1){
            
        
        int x = tab[t.pere[i]];
        if(x!=j){
        g.g[x].push_back(j);
         g.g[j].push_back(x);}
        
        }
        
        j++;

        }
    }
    while(!t.extraLink.empty())
    {
       pair<int,int> pair = t.extraLink.front();
       t.extraLink.pop_front();
       g.g[tab[pair.first]].push_back(tab[pair.second]);
       g.g[tab[pair.second]].push_back(tab[pair.first]);

 
    }
}
      
caract create_caract(graph & g)
{
    int deg1 = 0;
    int deg3 = 0;
    int degL = 0;
    int degMax = 0;

    for (int i = 0 ; i < g.n ; i++)
    {
        if(g.g[i].size()>degMax)
            degMax = g.g[i].size();
        if(g.g[i].size()==1)
            deg1++;
        if(g.g[i].size()==3)
            deg3++;    
        if(g.g[i].size()>3)
            degL++;
    }
    
    caract c;
    c.deg1 = deg1;
    c.deg3 = deg3;       
    c.degL = degL;
    c.degMax = degMax;
    return c;
}

bool isomorphique(graph g1, graph g2)
{
    int incr =0;
    if(g1.n != g2.n)
        return false;
    if(g1.m != g2.m)
        return false;
    int largest1 = 0;
    int largest2 = 0;
    for (int i = 0 ; i< g1.n ; i++){
        incr =0;
    for(vector<int>::iterator it = g1.g[i].begin(); it != g1.g[i].end(); ++it){
        incr++;
    }
        if (incr>largest1)
            largest1 = incr;
        incr =0;
    for(vector<int>::iterator it = g2.g[i].begin(); it != g2.g[i].end(); ++it){
       incr++; 
    }
        if (incr>largest2)
            largest2 = incr;
        
    }
    if(largest1!= largest2)
        return false;
    vector<vector<int> > t1;
    vector<vector<int> > t2;
    t1.resize(largest1+1);
    t2.resize(largest2+1);

    for (int i = 0 ; i< g1.n ; i++){
        incr =0;
    for(vector<int>::iterator it = g1.g[i].begin(); it != g1.g[i].end(); ++it){
        incr++;
    }
        t1[incr].push_back(i);
        incr =0;
    for(vector<int>::iterator it = g2.g[i].begin(); it != g2.g[i].end(); ++it){
       incr++; 
    }
         t2[incr].push_back(i);    
    }
    
        for (int i=0; i< largest1; i++){
            if(t1[i].size()!=t2[i].size())
                return false;
        }
    bool b = true;
   vector<int> tab;
    for  (int i = 0; i<g1.n;i++)
    {
        tab.push_back(i);
    }
    while (b)
    {
        if(directIsomorphe(g1,g2,tab)){
            return true;
        }
       b =  next_permutation(tab.begin(),tab.end());     
    }
   return false;
    
}

 bool directIsomorphe(graph g1, graph g2, vector<int> tab)
 {
      
     for(int i=0; i<g1.n; i++)
     {
         if (!sameNeighbors(g1,g2,i,tab[i],tab))
             return false;
     }
     return true;
 }
 
 bool sameNeighbors(graph g1,graph g2,int i,int j, vector<int> tab)
 {
     if(g1.g[i].size()!=g2.g[j].size())
         return false;
     
     for(vector<int>::iterator it = g1.g[i].begin(); it != g1.g[i].end(); ++it)
     {
         if(*find(g2.g[j].begin(),g2.g[j].end(),tab[*it])!=tab[*it])
             return false;     
     }
   return true;  
 }
 
  rdmgraph randomGraph(int n)
 {
      rdmgraph g;
      g.n = n;
      g.m = 0;      
      g.g.clear();
      g.g.resize(g.n);
      srand(time(NULL));
      tuple<int, int> tab[n];
      for(int i = 0 ; i <n; i++)
         tab[i] = make_tuple(rand() % n, rand() % n);
          for(int i=0; i<g.n;i++){
	  for(int j=0; j<i;j++){	
             if((abs(get<0>(tab[i])-get<0>(tab[j]))+abs(get<1>(tab[i])-get<1>(tab[j])))< n/80){    
                /*
                 * 70k-115
                 * 
                 * 
                 */
                 if(rand() % 2 == 0)
                 {
		g.g[i].push_back(j);
		g.g[j].push_back(i);
                g.m = g.m+1;
                 }
             }
         }
	}  
   return g;  
 }
  
 rdmgraph randomGraph3d(int n)
 {
      rdmgraph g;
      g.n = n;
      g.m = 0;      
      g.g.clear();
      g.g.resize(g.n);
      srand(time(NULL));
      tuple<int, int, int> tab[n];
      for(int i = 0 ; i <n; i++)
         tab[i] = make_tuple(rand() % n, rand() % n , rand() % n);
          for(int i=0; i<g.n;i++){
	 for(int j=0; j<i;j++){	
             if((abs(get<0>(tab[i])-get<0>(tab[j]))+abs(get<1>(tab[i])-get<1>(tab[j]))+abs(get<2>(tab[i])-get<2>(tab[j])))< n/28){    
                /*
                 * 70k-28
                 * 8k-7
                 * 20k - 10
                 */
                 if(rand() % 2 == 0)
                 {
		g.g[i].push_back(j);
		g.g[j].push_back(i);
                g.m = g.m+1;
                 }
             }
         }
	}  
   return g;  
 }
 
 tuple<graph,queue<int>,queue<queue<int>>,int> randomHubLam(rdmgraph rdmg,int k,int r,int a, int b,int maxl)
 { 
    graph g;
   
    queue<int> A;
    queue<int> copyA;
    queue<int> B;
   int select[rdmg.n];
   int close[rdmg.n];
   int deg[rdmg.n];
   
   for(int i=0; i< rdmg.n ; deg[i++] =0);
   for(int i=0; i< rdmg.n ; select[i++] =-1);
    queue<queue<int>> P;
   A.push(0);
   for(int i=0; i< rdmg.n ; close[i++] =-1);
   selectBall(r,0,&rdmg, select,  close, 0);
   int dist[rdmg.n];
   for(int i=0; i< rdmg.n ; dist[i++] =rdmg.n);
   for(int i=0;i<a-1;i++)
   {
    //   if(i!=a-2)
   // A.push(selectHub(&rdmg,A,dist));
    //   else
    A.push(selectHubL(&rdmg,A,dist,maxl));
    for(int i=0; i< rdmg.n ; close[i++] =-1);
   selectBall(r,A.back(),&rdmg, select,  close, A.back() );
   }
   copyA = A;
  
   queue<int> copy2A =A;
 
   B.push(A.front());
   A.pop();
   map<pair<int,int>,int> distHub;      
   fillDistHub(rdmg,copyA,&distHub);
  pair<int,int> curr;  
  
  
  
   while(!A.empty())
   {
    curr =  nextHub(&rdmg,A,B);
  cout << "construction chemin "  << curr.first << " et " << curr.second << endl; 
  deg[curr.first] =deg[curr.first] +1;
  deg[curr.second] =deg[curr.second] +1;  
  distHub[make_pair(curr.first,curr.second)] = g.n;
  distHub[make_pair(curr.second,curr.first)]= g.n;
    B.push(curr.first);
    A = deleteQueue(A,curr.first);
    
    P.push(makePath(&rdmg,curr,select));   
    queue<int> Q = P.back();
    while(!Q.empty()){
     for(int i=0; i< rdmg.n ; close[i++] =-1);
    selectBall(k,Q.front(), &rdmg,  select,  close, -2);  
    Q.pop();
   }
   }
   int nbLam = a-1;      
   while(nbLam<b)
   {
       int min = g.n;
      queue<int> copy2A = copyA; 
      while(!copy2A.empty()){
           queue<int> copy3A = copyA; 
           while(!copy3A.empty()){
               if(copy3A.front() != copy2A.front()){
               if(distHub[make_pair(copy2A.front(),copy3A.front())]<min)
               {
                   curr.first=copy2A.front();
                   curr.second=copy3A.front();  
                   min = distHub[make_pair(curr.first,curr.second)];                                                   
               }
               }
               copy3A.pop();
           }
           copy2A.pop();
      }
     deg[curr.first] =deg[curr.first] +1;
     deg[curr.second] =deg[curr.second] +1;  
     distHub[make_pair(curr.second,curr.first)] = g.n;
     distHub[make_pair(curr.first,curr.second)] = g.n;
     cout << "construction chemin "  << curr.first << " et " << curr.second << endl; 
     queue<int> P2 = makePath(&rdmg,curr,select); 
     if(!P2.empty()){
      P.push(P2);   
    queue<int> Q = P2;
    while(!Q.empty()){
     for(int i=0; i< rdmg.n ; close[i++] =-1);
    selectBall(k,Q.front(), &rdmg,  select,  close, -2);  
    Q.pop();
   }
            nbLam++; 
     }
     if(min == g.n && P2.empty())
           nbLam++;
         
   }
   queue<queue<int>> copyP = P;
   
   
    int size =0;
    for (int i = 0; i< rdmg.n ; i++)
    {
        if(select[i]!=-1)
            size++;
    }
    int assign[rdmg.n];
    g.n = size;
    g.m = 0;      
    g.g.clear();
    g.g.resize(g.n);
    size = 0;
      for (int i = 0; i< rdmg.n ; i++)
    {
       if(select[i]!=-1)  
       {
           assign[i]=size;
           size++;
       }         
     }
    for(int i =0; i< rdmg.n; i++)
    {
        if(select[i]!=-1){
          for(vector<int>::iterator it = rdmg.g[i].begin(); it != rdmg.g[i].end(); ++it)
         {
              if(select[*it]!=-1){
            g.g[assign[i]].push_back(assign[*it]);
            if(assign[i]<assign[*it])
                g.m++;
              }
           }
        }
    }   
    
    bool notHub[g.n];
    for(int i=0; i< g.n ; notHub[i++] =true);
    while(!copy2A.empty()){
       deletePartialBFS(r,assign[copy2A.front()],&g,notHub); 
       copy2A.pop();
    }
    
    
    int color[g.n];
    int dist2[g.n];
    for(int i=0; i< g.n ; color[i++] = -1);
    for(int i=0; i< g.n ; dist2[i++] = -1);

    queue<int> bigP;
    int it = 0;
    while(!P.empty())
    {
        queue<int> temp = P.front();
        while(!temp.empty())
        {
         bigP.push(assign[temp.front()]);
         color[assign[temp.front()]]=it;
         dist2[assign[temp.front()]]=0;
         temp.pop();
        }  
        it++;
        P.pop();
    }
    while(!bigP.empty())
    {
     int curr = bigP.front();
     for(vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end(); ++it){
         if(dist2[*it]==-1 && dist2[curr]<k)
         {
             dist2[*it]=dist2[curr]+1;
             color[*it]=color[curr];
             bigP.push(*it);
         }
     }
     bigP.pop();
    }
 
    
 for(int i =0 ; i<g.n ; i++){
  if(notHub[i])
    {  
       for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end();){
           if(notHub[*it] && color[*it]!=color[i] && color[*it]!=-1 && color[i]!=-1){
              g.g[i].erase(it);        
           }
           else
           {
               it++;
           }
            }      
  }   
}
    
    int deg2 =0;
    for(int i=0;i<rdmg.n;i++)
    {
        if(deg[i]==2)
            deg2++;
    }
    queue<int> q;
    while(!B.empty())
    {
        q.push(assign[B.front()]);
        B.pop();
    }
    queue<queue<int>> l;
    
     while(!copyP.empty())
     {
         queue<int> Qtemp;
         queue<int> Qcurr=copyP.front();
         while(!Qcurr.empty())
         {
             Qtemp.push(assign[Qcurr.front()]);
             Qcurr.pop();
         }
         l.push(Qtemp);
         copyP.pop();     
     }
    
   return make_tuple(g,q,l,deg2);
 }
 
queue<int>  makePath(rdmgraph * g,pair<int,int> curr,int * select)
{
    int start = curr.first;
    int end = curr.second;       
   queue<int> P;
   BFSTreeReel t;
	t.n = g->n;      
      
	for(int i=0; i<t.n; ++i) {
            t.pere.push_back(-1);
            t.fils.push_back(deque<int>());
            t.profondeur.push_back(-1);
            }
        
        queue<int> q;
	q.push(start); 	
        t.pere[start] = start;
        t.profondeur[start] = 0;	
        bool stop = false;
        
        
        while(!q.empty() && !stop){
		int p = q.front();
		q.pop();
		for(vector<int>::iterator it = g->g[p].begin(); it != g->g[p].end() && !stop; ++it){
                    if(*it == end )
                    {
                        t.pere[end]=p;
                        stop = true;
                    }
                    else if(t.pere[*it] ==-1 && select[*it]!=-2){
                       /* bool good = true;
                        for(vector<int>::iterator it2 = g->g[*it].begin(); it2 != g->g[*it].end() && !stop; ++it2){
                            if(select[*it2]==-2)
                                good=false;
                        }*/
                      //  if(good){
				t.pere[*it] = p;
                                t.profondeur[*it]=t.profondeur[p]+1;                             
                                t.fils[p].push_back(*it);                                                            
				q.push(*it);
                     //   }
                       
                       
                    }
		}
	}
        if(!stop)
          {
            cout << "chemin non construit" << endl;
            return P;
            
        }
        
        P.push(end);
        while(t.pere[end] != end)
        {
           end = t.pere[end];
            P.push(end);          
        }      

    return P; 
}
 
queue<int> deleteQueue(queue<int> A,int a)
 {
   queue<int> q;
   while(!A.empty())
   {
       if(A.front()!=a)
           q.push(A.front());
       A.pop();   
   }
   return q;    
 }
  
pair<int,int> nextHub(rdmgraph* g, queue<int> A, queue<int> B)
 {
   int dist[g->n];
   for(int i=0; i< g->n ; dist[i++] =g->n);
   pair<int,int> result = make_pair(B.front(),B.front());
   int minDist = g->n;
   
    while(!B.empty())
   {
   int b = B.front();
   B.pop();
   updateDist(b,dist,g,0);
   queue<int> q = A;
    while(!q.empty())
   {        
     int a = q.front();
     q.pop();
     if(dist[a]<minDist)
     {
        minDist =  dist[a];
        result = make_pair(a,b);
     }
   }
      for(int i=0; i< g->n ; dist[i++] =g->n);
   } 
   
   
   
   return result;
 }
 
int selectHub(rdmgraph* g, queue<int> A, int * dist){
   queue<int> q = A;   
   while(!q.empty())
   {
       int a = q.front();
       q.pop();
       updateDist(a,dist,g,0);
   }
   int result = 0;
   for(int i=0; i<g->n ; i++){
       if(dist[i]>dist[result] && dist[i]<g->n)
           result = i;
           
   }
   cout << "hub : " << result << " dist : " << dist[result] << endl;
   return result;
}

int selectHubL(rdmgraph* g, queue<int> A, int * dist, int l){
   queue<int> q = A;   
   while(!q.empty())
   {
       int a = q.front();
       q.pop();
       updateDist(a,dist,g,0);
   }
   int result = 0;
   for(int i=0; i<g->n ; i++){
       if(dist[i]>dist[result] && dist[i]<g->n && dist[i]<=l)
           result = i;
           
   }
   cout << "hub : " << result << " dist : " << dist[result] << endl;
   return result;
}


void  updateDist(int curr,int * dist,rdmgraph* g, int l){
    dist[curr]=0;
    queue<int> queue;
    queue.push(curr);
    /* for(vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end(); ++it)
     {
         if(dist[*it]>l+1)
             updateDist(*it,dist,g,l+1);
     }*/
    
    
     while(!queue.empty())
    {
        int s = queue.front();
        queue.pop();
 
        
     for(vector<int>::iterator it = g->g[s].begin(); it != g->g[s].end(); ++it)
        {
            if(dist[*it]>dist[s]+1)
            {
                dist[*it] = dist[s]+1;
                queue.push(*it);
            }
        }
    }
    return;
}

void selectBall(int profondeur, int curr, rdmgraph *g, int* select, int* close, int color){
  close[curr]=profondeur;
  if(select[curr]<0)
  select[curr]=color;
  if (profondeur ==0)
      return;

  for (std::vector<int>::iterator it = g->g[curr].begin(); it != g->g[curr].end();it++){
          if(close[*it]<profondeur)
     selectBall(profondeur-1,*it,g,select,close,color);           
     }
   
    return;
}

squelette simpleSquelette(graph g,queue<int> q, int r){
    squelette s;
    s.n=g.n;
    s.m =g.m;
    s.g = g.g;
    int close[g.n];
    int n = s.n;

    for (int i =0 ; i<n ; i++)
    {
      s.closest.push_back(n+1);
      s.father.push_back(-1);
      s.color.push_back(-1);
      s.hub.push_back(-1);   
      s.bfs.push_back(-1);
    }
    
    while(!q.empty())
    {
        int p = q.front();
        q.pop();
        for(int i = 0; i < g.n ; close[i++] = -1);
        colorClose(r,p,p,close,&s);
    }
   return s; 
}


 
 void colorClose(int profondeur, int color, int curr, int* close, squelette *s){
  close[curr]=profondeur;
  s->hub[curr]=color;
  if (profondeur ==0)
      return;

  for (std::vector<int>::iterator it = s->g[curr].begin(); it != s->g[curr].end();it++){
          if(close[*it]<profondeur)
     colorClose(profondeur-1, color,*it, close, s);           
     }
   
    return;
}
 
graph computeQuotient(queue<int> q, queue<queue<int>> l,int n)
{
    graph g;
    g.n=q.size();
    g.m=l.size();
    g.g.resize(g.n);
    int tab[n];
    int curr =0;
    while(!q.empty())
    {
        tab[q.front()]=curr;
        q.pop();
        curr++;       
    }

    while(!l.empty())
    {
        g.g[tab[l.front().front()]].push_back(tab[l.front().back()]);
        g.g[tab[l.front().back()]].push_back(tab[l.front().front()]);
        l.pop();
    }

    return g;
}

 graph removeDeg2(graph g){
   int tab2[g.n];
   int nbDeg2 =0;
   int temp = 0;
  for(int i=0; i< g.n ; tab2[i++] =-1); 
   for(int i= 0; i<g.n ; i++)
   {
       if(g.g[i].size()==2)
       {
         nbDeg2++;
         int vois1,vois2;
         bool tempB=true;
          for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end();it++){
              if(tempB){
              vois1=*it;
              tempB=false;}
              else
              {
               vois2=*it;
              }        
           } 
         tempB=true;
           for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end();it++){
                for (std::vector<int>::iterator it2 = g.g[*it].begin(); it2 != g.g[*it].end();it2++){
                    if(*it2 ==i)
                    {
                       if(tempB)
                       {
                          *it2=vois2;
                         tempB=false;  
                       }  
                       else
                       {
                          *it2=vois1;
                       }                       
                    }
               }  
           }     
       }   
       else
       {
           tab2[i]=temp;
           temp++;
       }
   }
      
    graph g2;
    g2.n=temp;
    g2.m=g.m-nbDeg2;
    g2.g.resize(g2.n);   
    
    bool tab3[g.n];
    for(int i=0; i< g.n ; tab3[i++] =true); 

    for(int i=0;i<g.n;i++)
    {
        if(g.g[i].size()!=2)
        {
          for (std::vector<int>::iterator it = g.g[i].begin(); it != g.g[i].end();it++){
          if(tab2[i]==tab2[*it]){
              if(tab3[tab2[i]]){
              g2.g[tab2[i]].push_back(tab2[*it]);
              tab3[tab2[i]]=false;
              }
          }
          else
               g2.g[tab2[i]].push_back(tab2[*it]);
           }         
        }       
    }
     return g2;
 }



 int minDist(graph g, queue<int> A){
     int dist[g.n];
     int curr;
     queue<int> q = A;
     queue<int> q2;
     int min=g.n;
     while(!q.empty()) 
     {
       for(int i=0; i< g.n ; dist[i++] =-1);
       curr = q.front();
       dist[curr]=0;
       q2.push(curr);     
       while(!q2.empty())
       {
           curr = q2.front();
           for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
               if(dist[*it]==-1){
                   dist[*it]=dist[curr]+1;
                   q2.push(*it);
               }
           }         
           q2.pop();
       }
       q2=A;
       while(!q2.empty())
       {
           if(dist[q2.front()]<min && dist[q2.front()]>0)
               min = dist[q2.front()];
           q2.pop();
       }      
      q.pop();
     }
     return min;
 }
 
 int distToHub(graph g,int start,queue<int> A){
      bool hubs[g.n];
      int dist[g.n];
     for(int i=0; i< g.n ; hubs[i++] =false);
   for(int i=0; i< g.n ; dist[i++] =-1);
     while(!A.empty())
     {
        hubs[A.front()]=true;
        A.pop();
     }
      dist[start]=0;
      queue<int> q;
      q.push(start);
      int curr;
      while(!q.empty())
      {
          curr = q.front();
          if(hubs[curr])
              return dist[curr];
           for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
            if(dist[*it]==-1)      
              {
                  dist[*it]=dist[curr]+1;
                  q.push(*it);
              }
            }
          q.pop();
      }
      return -1;
  }

 
 void fillDistHub(rdmgraph g,queue<int> A,map<pair<int,int>,int>* m)
 {
     bool hubs[g.n];
     int dist[g.n];
     queue<int> copyA = A;
     for(int i=0; i< g.n ; hubs[i++] =false);
      while(!copyA.empty())
     {
        hubs[copyA.front()]=true;
        copyA.pop();
     }
     while(!A.empty())
     {
        int start = A.front();
      for(int i=0; i< g.n ; dist[i++] =-1);  
      dist[start]=0;
      queue<int> q;
      q.push(start);
      int curr;
      int stop = A.size();
   //   while(!q.empty() && stop >0)
       while(!q.empty())
      {      
          curr = q.front();
          if(hubs[curr] && curr!=start){
                    (*m)[make_pair(start,curr)]= dist[curr];
                    (*m)[make_pair(curr,start)]= dist[curr];      
                     stop--;
          }
           for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
            if(dist[*it]==-1)      
              {
                  dist[*it]=dist[curr]+1;
                  q.push(*it);
              }
            }
          q.pop();
      }       
             A.pop();             
     }
     return;
 }
 
 
 label fillLabel(graph g,queue<int> A,queue<queue<int>> qL , int r)
 {
     int nbHub = A.size();
     int curr;
     label lab;
     lab.n = g.n;
     lab.hub.resize(lab.n);
     lab.H.resize(lab.n);
     lab.L.resize(lab.n);
     lab.map.resize(lab.n);
     queue<int> q = A;
     int it=0;
     while(!q.empty())
     {
         lab.map[q.front()]=it;
         it++;
         q.pop();
     }
     q=A;
     
     
     for(int i =0 ; i<lab.n ; lab.hub[i++]=false);
     for(int i =0 ; i<lab.n ; lab.L[i++]=make_tuple(-1,-1,-1,-1,-1));
     for(int i =0 ; i<lab.n ; i++)
     {
         for(int j=0; j< nbHub; j++)
          lab.H[i].push_back(-1);
     }
     
   

     while(!q.empty())
     {
         int hubCurr=q.front();
         lab.H[hubCurr][lab.map[hubCurr]]=0;
         queue<int> q2;
         q2.push(hubCurr);
         
         while(!q2.empty()){
             curr =q2.front();
         for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
            if(lab.H[*it][lab.map[hubCurr]]==-1)     
              {
                  lab.H[*it][lab.map[hubCurr]]=lab.H[curr][lab.map[hubCurr]]+1;
                  if(lab.H[*it][lab.map[hubCurr]]<=r)
                  lab.hub[*it]=true;
                  q2.push(*it);
              }
            }
             
         q2.pop() ;
      }  
      q.pop();              
     }
     
     
     
     queue<queue<int>> copyqL = qL;
    it=0;
    while(!copyqL.empty())
     {
         q = copyqL.front();
         int hub1= q.front();
         int hub2= q.back();
         if(hub1>hub2)
         {
             hub1 = hub2;
             hub2 = q.front();
         }
         int l = q.size();
         int d = 0;
         bool bo = hub1==q.front();
         while(!q.empty())
         {                 
             if(bo)
             lab.L[q.front()]=make_tuple(hub1,hub2,0,d,it);
             else
             lab.L[q.front()]=make_tuple(hub1,hub2,0,l-d,it);
             d++;
             q.pop();
         }
         it++;
         copyqL.pop();
     }
    
    
     
     queue<int> bigQ;
     
     
     while(!qL.empty()){
         q=qL.front();
         while(!q.empty())
         {
             bigQ.push(q.front());
             q.pop();
         }  
         qL.pop();
    } 
  
     while(!bigQ.empty())
     {            
       int curr = bigQ.front();  
       for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
           if(get<0>(lab.L[*it])==-1)
           {
               lab.L[*it] = make_tuple(get<0>(lab.L[curr]),get<1>(lab.L[curr]),get<2>(lab.L[curr])+1,get<3>(lab.L[curr]),get<4>(lab.L[curr]));
               bigQ.push(*it);
           }
       }
          bigQ.pop();       
     }
     
  return lab;   
 }
 
 int labelDist(label lab, int u, int v)
 {
     if(get<0>(lab.L[u]) == get<0>(lab.L[v]) && get<1>(lab.L[u]) == get<1>(lab.L[v]))
     {
        return get<2>(lab.L[v])+get<2>(lab.L[u])+fabs(get<3>(lab.L[v])-get<3>(lab.L[u]));
     }
     int min = lab.n;
     
     for(int i = 0 ; i<lab.H[u].size();i++){
              if(lab.H[u][i]+lab.H[v][i] <min)
                  min = lab.H[u][i]+lab.H[v][i];         
     }  
     return min;
 }
 
 