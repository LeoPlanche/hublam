#include "dsu.hpp"

//TODO
//incoporate rank

DSU::DSU(int n): n(n){
	par = new int [n];
	for(int i=0;i<n;++i) par[i] = i;
}

DSU::~DSU(){
	delete [] par;
}

void DSU::reinit(){
	for(int i=0;i<n;++i) par[i] = i;
}

int DSU::find(int i){
	if(par[i] != i){
		return par[i] = find(par[i]);
	}
	return i;
}
	
void DSU::merge(int x,int y){
	par[find(x)] = find(y);
}

