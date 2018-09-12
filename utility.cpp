#include "utility.hpp"

#include <vector>
#include <cstdlib>
#include <cmath>

//debug
#include<cassert> 

using namespace std;

Color::Color(int r,int b,int g){
	c[0] = r;
	c[1] = b;
	c[2] = g;
}

int Color::get(int i) const {
	assert(0 <= i && i < 3);
	return this->c[i];	
}

void Color::set(int i, int c){
	assert(0 <= i && i < 3);
	this->c[i] = c;
}

void generate_colors_randomly(vector<Color> & colors, int n){
	for(int i=0;i<n;++i){
		int r = rand() % 256;
		int b = rand() % 256;
		int g = rand() % 256;
		colors.push_back(Color(r,b,g));	
	}
}

Color getPattern(int index) {
	int n = cbrt(index);
	index -= (n*n*n);

	Color res(n,n,n);
	if (index == 0) {
		return res;
	}				
	index--;
	int v = index % 3;
	index = index / 3;
	if (index < n) {
		res.set(v,index % n);
		return res;
  }
	index -= n;
	res.set(v,index / n);
	++v;
	res.set(v%3,index % n);
	return res;
}

int getElement(int index) {
  int value = index - 1;
  int v = 0;
  for (int i = 0; i < 8; i++) {
	  v = v | (value & 1);
    v <<= 1;
    value >>= 1;
  }
	v >>= 1;
  return v & 0xFF;
}

/* Won't work if n is very large.
 * Taken from SO
 */

void generate_colors_deterministically(vector<Color> & colors, int n){
	for(int i=0;i<n;++i){
		Color c = getPattern(i);
		for(int j=0;j<3;++j){
			c.set(j,getElement(c.get(j)));
		}
		colors.push_back(c);
	}
}

void generate_colors(vector<Color> & colors, int n, bool op){
	if(op) generate_colors_randomly(colors,n);
	else generate_colors_deterministically(colors,n);
}

ostream & operator << (ostream & os, Color const &c){
	os << c.get(0) << "," << c.get(1) << "," << c.get(2) ;
	return os;
}
