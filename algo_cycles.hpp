#ifndef ALGO_CYCLES_HPP_INCLUDED
#define ALGO_CYCLES_HPP_INCLUDED

#include "GraphIO.hpp"
#include <vector>

//finds a largest cycle with no chords superieur to k
//the chord length is define by the distance of the endpoints on the cycle

void algo_cycles(graph & g, int d, int k,vector<int> & assign);

#endif // ALGO_CYCLES_HPP_INCLUDED
