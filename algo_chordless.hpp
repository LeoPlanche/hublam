#ifndef ALGO_CHORDLESS_HPP_INCLUDED
#define ALGO_CHORDLESS_HPP_INCLUDED

#include "GraphIO.hpp"
#include <vector>

/* returns all chordless cycles of a Graph
 * Watch out for large n as this uses adjacency matrix
 * and running time is potentielly exponential */

void chordless_cycles(graph & g, vector<vector<int> > &cycles);


#endif // ALGO_CHORDLESS

