#ifndef ALGO_INTERVAL_HPP_INCLUDED
#define ALGO_INTERVAL_HPP_INCLUDED

#include "GraphIO.hpp"
#include "algo.hpp"
#include <vector>
#include <utility>

using namespace std;

bool check_interval(graph & g);

void calculate_intervals(vector<vector<pair<int,int> > > & intervals,cliqueTree & ct);

int diametral_path(graph & g,int k);//vector<int> &);

#endif

