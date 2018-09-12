#ifndef DSU_HPP_INCLUDED
#define DSU_HPP_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

class Color{
	int c[3];	

	public:
	Color(int,int,int);
	void set(int i, int c);
	int get(int i) const ;	

	friend ostream & operator << (ostream & os, Color const &);
};

ostream & operator << (ostream & os, Color const &);

/* generates n colors 
 * op true -> deterministically, randomly otherwise
 * */

void generate_colors(vector<Color> &, int n, bool op);

#endif
