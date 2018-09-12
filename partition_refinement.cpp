#include <cstdlib>
#include <cassert>
#include <iostream>

#include "partition_refinement.hpp"

PartRef::PartRef(vector<int>& ini) : init(ini) {
	this->n = ini.size();
	this->timer = 0;
	per.resize(n);
	inv.resize(n);
	used.resize(n,false);
	elements.resize(n);


	elements[ini[0]] = first = last = new Elem(ini[0],NULL);
	begin = current = end = new Klass(timer,first);
	first->setParent(begin);

	for(int i=1;i<n;++i){
		elements[ini[i]] = new Elem(ini[i],current);
		current->insertElemEnd(elements[ini[i]]);
	}
}

PartRef::~PartRef(){
	for(int i=0;i<n;++i){
		delete elements[i];
	}
	Klass * ite = begin; 
	while(ite){ 
		Klass * next = ite->getNext();
		delete ite;
		ite = next;
	}
}


void PartRef::partition(vector<vector<int> > &pivots){
  ++timer;

	for(int i=0;i<n;++i){
		int pivot = current->getFirst()->getId();
		//used could be remove because of inv
		used[pivot] = true;
		per[i] = pivot;
		inv[pivot] = n-i-1;

		//TODO
		/*current->print(current);
		cout << endl;*/
		//begin->print(begin);
		//cout << endl << endl;

		//partition no longer interesting
		if(current->checkOnly(elements[pivot])){
			current->setStamp(-i-1);
			current = current->getNext();
		// remove first element from partition
		}else{
			if(current == begin)
				begin = current->insertElemBeforeSplit(-i-1,current->getFirst());
			else 
				current->insertElemBeforeSplit(-i-1,current->getFirst());
		}


		for(vector<int>::iterator it = pivots[pivot].begin();
										it != pivots[pivot].end(); ++it){
			if(!used[*it]){

				Elem * e = elements[*it];
				Klass * old_parent = e->getParent();
				if(old_parent == current){
					current = old_parent->insertElemBeforeSplit(timer,e);
				}else{
					old_parent->insertElemBeforeSplit(timer,e);
				}
				//Klass became empty
				if(old_parent->getFirst() == NULL){
					delete old_parent;
				}
			}
		}
		++timer;
	}
}
