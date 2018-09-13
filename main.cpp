#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <string> 
#include <cmath>



#include "algo_BFS.hpp"
#include "algo.hpp"
#include "algo_clique.hpp"
#include "algo_interval.hpp"
#include "algo_clustering.hpp"
#include "algo_branches.hpp"
#include "algo_cycles.hpp"
#include "algo_chordless.hpp"
#include "GraphIO.hpp"
#include "algo_union.hpp"
#include "dirent.h"

using namespace std;

/*
A more precise description of functions may be found in header files 
 */

int main(int, char * argv[]){

         /* initialize random seed: */
        srand (time(NULL));
        /* a graph */
	graph g;
        /* a struct to handle graphs generated with randomGraph*/
        rdmgraph g2;
        /* reads a graph and put it in g*/
        bool  b = read_graph(g,"C:\\path\\mygraph");
        int k,r;
        /* find the first hub in the graph g and put it in s*/
       int s = findFirstHub(g,k,r);
       /* find every hubs of g and put them in q*/
       queue<int> q = findhubs(g,k,r,s);
       /* find every laminar paths and put them in l*/
       queue<queue<int>> l = findlaminars(g,k,r,q);
        /* generates a random graph of at most n nodes, depending on the average degree*/
        int n;
        g2 = randomGraph(40000);
        /* generates from a random graph a graph with an hub laminar decomposition*/
        int nbHubs, nbLams, maxL;
        tuple<graph,queue<int>,queue<queue<int>>,int> pair = randomHubLam(g2,k,r,nbHubs,nbLams,maxL);
        /* colors the graph with the previously computed hub-laminar decomposition 
         writes the result in DOT format with hubs in red and laminars in blue*/
         squelette sq = simpleSquelette(g,q,r);
         write_squelette(sq,":\\path\\mygraphcolored.dot");

	return 0;
        }

