#ifndef GRAPH_IO_HPP_INCLUDED
#define GRAPH_IO_HPP_INCLUDED


#include <vector>
#include <tuple>
#include <map>
#include <queue>

// graph input output library

using namespace std;

struct caract{
    int deg1;
    int deg3;
    int degL;
    int degMax;
};
struct graph{
	int n,m;
	vector<vector<int> > g;
};

struct rdmgraph{
	int n,m;
        vector<tuple<int, int> > coord;
        vector<tuple<int, int, int> > coord3d;
	vector<vector<int> > g;
};



struct graph_edge_nb{
	int n,m;
	vector<vector<pair<int,int> > >g;
};

struct edges{
	int n,m;
	vector<int> u,v;
};

struct mapping{
	graph g;
	edges e;
	map<int,int> m;
};

struct mapping_edge_nb{
	graph_edge_nb g;
	edges e;
	map<int,int> m;
};

struct CovId{
	vector<double> covs;
	vector<double> ids;
};

struct BFSTree{
	int n;
        int cycle;
	vector<deque<int> > fils;
        vector<int> status;
        vector<int> pere;
        vector<vector<pair<int,int> > > voisins;
        deque<pair<int,int> >extraLink;
};

struct BFSTreeReel{
	int n;
	vector<deque<int>> fils;
        vector<int> profondeur;
        vector<int> pere;
};

struct squelette{
	int n,m;
	vector<vector<int> > g;
        vector<int> color;
        vector<int> closest;
        vector<int> father;
        vector<int> bfs;
        vector<int> hub;
        vector<int> last;
        queue<int> qLast;

};

struct relique{
	int n,m;
	vector<vector<int> > g;
        vector<int> length;
};


struct label{
	int n;
        vector<bool> hub;       
        vector<int> map;       
        vector<vector<int>> H; // a modif
        vector<tuple<int,int,int,int,int>> L; // tableau[n][4]??
};

/* Reads a graph with n m on first line.
 * edges list follows ( m lines)
 * Node id's must be 0 <= id < n
 */

bool read_graph(graph & g,const char * file);

bool write_graph(graph & g,const char * file);

bool write_squelette(squelette & g,const char * input);


bool read_graph_edge_nb(graph_edge_nb & g, char * f);

/* As read_graph above but node id's can be arbitrary.
 * Creates mapping (see above) between original and new nodes
 */
bool read_cc(mapping& m, char * file);

bool read_cc_edge_nb(mapping_edge_nb & m, char * file);

bool read_edges(edges & e, char * file);

bool read_graph_coloring(graph & g, edges &, int &, 
								vector<int> &col, char * file, int);

bool create_gephi_coloring(edges & e, const char * file, 
								int nbCommunities, vector<int>& assign);

bool get_maxCover_and_id(const char * input, CovId &);

BFSTree clean_tree(BFSTree t, int k);

BFSTree clean_treeV2(BFSTree t, int k);

squelette algoReel(graph t, int k, int r);

graph createRelique(squelette s);


BFSTreeReel BFS_tree_reel(graph & g, int start);


int deepestUncolored(squelette *s, BFSTreeReel *t);

bool hubClose(int r, int i ,squelette * s);


bool son(int k,int start,int goal,BFSTree * t);


bool createExtraLink(int k, int start, BFSTree * );

bool valid(int source, BFSTree * t);

void toClosest(int k, int start, BFSTree * t);

bool pere(int k, int start, int curr, BFSTree * t);

void elaguer1(int k, int start, BFSTree *t);

void elaguerReel(int k, int profondeur, int color, int curr, int start, squelette *s);

void fillHub(int h,int k,int r,squelette *s);

void fusionHub(int h,int l,squelette *s );

void create_graph_from_tree(graph & g, BFSTree t);

caract create_caract(graph & g);

bool isomorphique(graph g1, graph g2);

bool directIsomorphe(graph g1, graph g2, vector<int> tab);

bool sameNeighbors(graph g1,graph g2,int i,int j, vector<int> tab);

void colorPartialBFS(int profondeur, int color, int curr, graph *g, int* tabCol);

queue<int> findhubs(graph g, int k, int r, int s);

pair<int,int> uncolored(int profondeur,queue<int> A,graph *g,int* tabCol);

//int uncoloredClose(int profondeur,int curr,graph *g,int* tabCol, int* close);

 pair<queue<int>,BFSTreeReel> stopBFS(pair<int,int> curr,graph *g,int* tabColor);

void  colorBFS(BFSTreeReel t,int start,int* tabColor);

int hubMiddle(queue<int> P, int color, graph * g,int k, int* tabColor);

bool searchCandidate(int k,int color,int curr,graph * g,int* close, int* close2,int* tabColor);

bool closeToA(int curr,int c,int profondeur ,graph* g, int* tabColor);

 void colorLamCycle(int profondeur, queue<int> P, int hub1,int hub2,int * tabColor, graph * g);
 
 void colorLamCycleK(int pronfodeur,int curr,int hub1,int hub2,int * tabColor,bool * treated, graph * g);
 
 bool adjacentToA(int f,graph * g,int* tabColor);
 
  void deletePartialBFS(int profondeur, int curr, graph *g, bool* deleted);
  
    void deleteTruePartialBFS(int profondeur, int curr, graph *g, bool* deletable, int* close);
  
  bool undeletedClose(int profondeur,int curr,graph *g,bool* deleted, int* close);
  
int undeleted(int profondeur,queue<int> A,graph *g,bool* deleted, bool* deletable);

 bool undeletedTrueClose(int profondeur,int curr,graph *g,bool* deleted, int* close);
  
int trueUndeleted(int profondeur,queue<int> A,graph *g,bool* deleted);

  queue<queue<int>> findlaminars(graph g, int k, int r, queue<int> A);
  
  queue<int> computePath(int start,queue<int> A,graph *g,bool* deleted,  bool * deletable);

  void deletePath(int profondeur, queue<int> P, graph * g, bool * deletable,bool *deleted );
  
  void deletePathK(int profondeur,int curr,graph * g,bool * deletable, bool * deleted, int * close );
  
  void  deleteBFS(BFSTreeReel t,int start,bool* deleted);

  bool isKLamiar(graph g, int k);

  int findFirstHub(graph g, int k, int r);
  
  void colorPath(int profondeur, int curr, graph *g, int* close);

  rdmgraph randomGraph(int n);
  
  rdmgraph randomGraph3d(int n);

  void  updateDist(int curr,int * dist,rdmgraph* g, int l);

  void selectBall(int profondeur, int curr, rdmgraph *g, int* select, int* close, int color);
  
int selectHub(rdmgraph* g, queue<int> A, int * dist);

int selectHubL(rdmgraph* g, queue<int> A, int * dist, int l);


 tuple<graph,queue<int>,queue<queue<int>>,int> randomHubLam(rdmgraph rdmg,int r,int k,int a, int b, int maxl);
 
 queue<int>  makePath(rdmgraph * g,pair<int,int> curr, int * select);

 queue<int> deleteQueue(queue<int> A,int a);

pair<int,int> nextHub(rdmgraph* g, queue<int> A, queue<int> B);

squelette simpleSquelette(graph g,queue<int> q, int r);

 void colorClose(int profondeur, int color, int curr, int* close, squelette *s);
 
 bool isConnex(graph g);
 
 graph computeQuotient(queue<int> q, queue<queue<int>> l, int n);
 
 graph removeDeg2(graph g);
 
 int minDist(graph g, queue<int> A);
 
 int distToHub(graph g,int start,queue<int> A);
 
  void fillDistHub(rdmgraph g,queue<int> A,map<pair<int,int>,int>* m);

 label fillLabel(graph g,queue<int> A,queue<queue<int>> qL, int r);

  int labelDist(label lab, int u, int v);


#endif // GRAPH_IO_INCLUDE
