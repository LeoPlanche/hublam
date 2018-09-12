#include <cstdlib>
#include <cassert>

#include "partition_refinement_interval.hpp"

//debug
#include <cassert>
#include <iostream>

using namespace std;

//debug function 
void print_cliques(Klass * k, vector<vector<int> > & cliques){
	Elem * e = k->getFirst();
	while(e){
		int id = e->getId();
		if(e->getPrev() == NULL || e->getParent() != e->getPrev()->getParent()){
			cout << " | ";
		}else{
			cout << "  ";
		}
		for(size_t i=0;i<cliques[id].size();++i){
			cout << cliques[id][i]+1;	
		}
		e = e->getNext();
	}
	cout << endl << endl;
}

PartRefInter::PartRefInter(int n, vector<vector<int> > & v_in_c, vector<vector<int> > & edges,
									vector<vector<int > > & seperators, vector<vector<int > > & cliques) 
				: vertices_in_clique(v_in_c), edges(edges), seperators(seperators), cliques(cliques){

	this->n = n;
	this->nbCliques = edges.size();
	this->timer = 0;
	this->counter = 0;
	//per.resize(n);
	//inv.resize(n);
	used_pivots.resize(v_in_c.size(),false);
	used_edges.resize(edges.size()-1,false);
	edge_count.resize(edges.size()-1,0);
	elements.resize(nbCliques);

	assert(nbCliques-1 == (int)seperators.size());

	elements[nbCliques-1] = first = last = new Elem(nbCliques-1,NULL);
	begin = current = end = new Klass(timer,first);
	first->setParent(begin);

	for(int i=1;i<nbCliques;++i){
		elements[nbCliques-i-1] = new Elem(nbCliques-i-1,current);
		current->insertElemEnd(elements[nbCliques-i-1]);
	}
}

PartRefInter::~PartRefInter(){
	for(int i=0;i<nbCliques;++i){
		delete elements[i];
	}
	Klass * ite = begin; 
	while(ite){ 
		Klass * next = ite->getNext();
		delete ite;
		ite = next;
	}
}

void PartRefInter::incrementEdgesCount(vector<int> & edges){
	for(vector<int>::iterator it = edges.begin(); it != edges.end(); ++it){
		++edge_count[*it];
	}
}

void PartRefInter::partition(vector<int> & clique_order){
  ++timer;
	while(counter < nbCliques){//&& current != NULL){
		//partition no longer interesting
		if(current->checkOnly(current->getFirst())){
			current->setStamp(counter);
			current = current->getNext();
			++counter;
		}else{
			if(pivots.empty()){
				// create class front
				// with one element
				if(current == begin){
					begin = current->insertElemBeforeSplit(timer,current->getFirst());
				}else{
					current->insertElemBeforeSplit(timer,current->getFirst());
				}
				++counter;
				++timer;
				// update pivots
				assert(current->getPrev() != NULL);
				int cli = current->getPrev()->getFirst()->getId();

				for(vector<int>::iterator eit = edges[cli].begin();
												eit != edges[cli].end(); ++eit){
					if(!used_edges[*eit]){
						used_edges[*eit] = true;
						for(vector<int>::iterator vit = seperators[*eit].begin();
											vit != seperators[*eit].end(); ++vit){
							if(!used_pivots[*vit]){
								used_pivots[*vit] = true;
								pivots.push(*vit);
							}
						}
					}
				}

			}else{
				int pivot = pivots.top();
				pivots.pop();

				//TODO ask about optimization 
				
				// mark all Klasses that contains pivots
				for(vector<int>::iterator it = vertices_in_clique[pivot].begin();
												it != vertices_in_clique[pivot].end(); ++it){
					elements[*it]->getParent()->setStamp(timer);	
				}
				++timer;
				
				// get first and last Klass
				Klass * pivot_first = NULL, *pivot_last =NULL;
				int timer_first = timer, timer_last = timer +1;
				//int got = 0;
				//might incoporate a break
				for(vector<int>::iterator it = vertices_in_clique[pivot].begin();
												it != vertices_in_clique[pivot].end(); ++it){
					if(!elements[*it]->getParent()->checkPrevSameStamp()){
						pivot_first = elements[*it]->getParent();	
					}
					if(!elements[*it]->getParent()->checkNextSameStamp()){
						pivot_last = elements[*it]->getParent();
					}
				}
				//this might fail on non-interval graphs
				//because the partial order might not be preserved
				//assert(pivot_first != pivot_last);
				assert(pivot_first != NULL);
				assert(pivot_last  != NULL);

				//move cliques 
				//mark tree edges
				//TODO check if this rests linear
				//vector<int> possible_edges;
				Klass * tmp;
				for(vector<int>::iterator it = vertices_in_clique[pivot].begin();
												it != vertices_in_clique[pivot].end(); ++it){

					if(elements[*it]->getParent() == pivot_first){
						//insert klass end
						//new inserted elements in klass at the end
						//keep ordering
						//delete parent maybe
						// update current if necessary
						if(current == pivot_first){
							tmp = pivot_first->insertElemAfterSplit(timer_first,elements[*it]);
						}else{
							pivot_first->insertElemAfterSplit(timer_first,elements[*it]);
						}

						if(pivot_first->getFirst() == NULL){
							if(current == pivot_first) current = tmp;
							delete pivot_first;
						}

						incrementEdgesCount(edges[*it]);
						//possible_edges.insert(possible_edges.end(),edges[*it].begin(),edges[*it].end());
					}
					if(elements[*it]->getParent() == pivot_last){
						//insert klass beginning
						//new inserte elements in klass at the end
						//keep ordering
						//delete parent maybe
						//update current if necessary
						if(current == pivot_last){
							current = pivot_last->insertElemBeforeSplit(timer_last,elements[*it]);
						}else{
							pivot_last->insertElemBeforeSplit(timer_last,elements[*it]);
						}
						if(pivot_last->getFirst() == NULL){
							delete pivot_last;
						}

						incrementEdgesCount(edges[*it]);
					}
				}
				timer+=2;

				//update pivots
				//could probably be optimized
				for(vector<int>::iterator it = vertices_in_clique[pivot].begin();
												it != vertices_in_clique[pivot].end(); ++it){

					if(elements[*it]->getParent() == pivot_first ||
							elements[*it]->getParent() == pivot_last){
						for(vector<int>::iterator eit = edges[*it].begin();
														eit != edges[*it].end(); ++eit){
							if(!used_edges[*eit] && edge_count[*eit] % 2 == 1){
								used_edges[*eit] = true;
								for(vector<int>::iterator vit = seperators[*eit].begin();
													vit != seperators[*eit].end(); ++vit){
									if(!used_pivots[*vit]){
										used_pivots[*vit] = true;
										pivots.push(*vit);
									}
								}
							}
						}
					}
				}	
			}
		}
	}

	Klass * tmp = begin;
	int i = 0;
	clique_order.resize(nbCliques);
	while(tmp){
		assert(tmp->checkOnly(tmp->getFirst()));
		assert( i < nbCliques);
		clique_order[i] = tmp->getFirst()->getId();	
		tmp = tmp->getNext();
		++i;
	}
}
