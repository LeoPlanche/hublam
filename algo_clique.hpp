#ifndef ALGO_CLIQUE_HPP_INCLUDED
#define ALGO_CLIQUE_HPP_INCLUDED

#include "GraphIO.hpp"

int diameter(graph & g);

bool diameter_check(graph & g, int k);

/* Algorithm as described in 
 * Oon computing the diameter of real-world undirected graphs.
 * Crescenzi et al.
 */

void FOURsweep(graph & g, int & u, int & lb);

/* Algorithm as described in 
 * Oon computing the diameter of real-world undirected graphs.
 * Crescenzi et al.
 *
 * With worst case O(nm), but faster in practice.
 * Optimal when k == 0.
 */

int iFUB(graph & g, int u, int lb, int k);

int diameter_iFUB(graph & g);

#endif // ALGO_CLIQUE

