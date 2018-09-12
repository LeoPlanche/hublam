#ifndef PARTITION_REFINEMENT_HPP_INCLUDED
#define PARTITION_REFINEMENT_HPP_INCLUDED

#include <vector>

#include "Elem.hpp"
#include "Klass.hpp"

//partition refinement as described in habib et al.

using namespace std;

struct PartRef{

	int n;
	int timer;
	vector<int> & init;
	vector<int> per,inv;
	vector<bool> used;
	vector<Elem*> elements;
	Klass * begin, * end, *current;
	Elem * first, * last;

	PartRef(vector<int> & init);
	~PartRef();
	void partition(vector<vector<int> > & pivots);

};

#endif // PARTITION_REFINEMENT_HPP_INCLUDED 

