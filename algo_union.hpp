#ifndef ALGO_UNION_HPP_INCLUDED
#define ALGO_UNION_HPP_INCLUDED

#include "GraphIO.hpp"
#include "algo_interval.hpp"
#include <vector>
#include <string>

/* checks if a decomposition is too small meaning a laminar graphs or 
 * cycle without large chords
 * gets separated into two parts */

void unify(graph & g, edges & e, int & , vector<int> &);

/* outputs all cycles colored in output.cycle_i.gdf
 *
 */

void find_cycles(graph & g, edges & e, int & co, vector<int> & colors, string output);

#endif //ALGO_UNION_HPP_INCLUDED

