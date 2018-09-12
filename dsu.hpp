#ifndef DSU_HPP_INCLUDED
#define DSU_HPP_INCLUDED

class DSU{
	int n;
	int * par;

	public:

	DSU(int n);
	~DSU();

	void reinit();
	int find(int i);
	void merge(int x,int y);
};

#endif
