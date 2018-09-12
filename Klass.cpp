#include "Klass.hpp"

//debug
#include <iostream>
#include <cassert>

using namespace std;

	//debug function
void Klass::print(Klass * par){
  Elem * cur = par->first;
  while(cur){
    cout << "ID:" << cur->getId() << " ParentStamp ";
    cout << cur->getParent()->stamp <<endl;

    cur = cur->getNext();
  }
}

Klass::Klass(int stamp, Elem * e){
  this->stamp = stamp;
	prev = next = NULL;
	first = last = e;
}

int Klass::getStamp(){
	return stamp;
}

void Klass::setStamp(int stamp){
	this->stamp = stamp;
}

bool Klass::checkPrevSameStamp(){
	if(this->prev == NULL || this->prev->getStamp() != this->stamp) return false;
	return true;
}

bool Klass::checkNextSameStamp(){
	if(this->next == NULL || this->next->getStamp() != this->stamp) return false;
	return true;
}

void Klass::setLast(Elem * e){
	last = e;
}

Elem * Klass::getFirst(){
	return this->first;	
}

Klass * Klass::getPrev(){
	return this->prev;
}

Klass * Klass::getNext(){
	return this->next;
}

void Klass::remove(){
	if(this->prev != NULL){
		this->prev->next = this->next;
	}
	if(this->next != NULL){
		this->next->prev = this->prev;
	}
	this->prev = this->next = NULL;
	this->first = this->last = NULL;
}

//checks if e is the only element in Klass
bool Klass::checkOnly(Elem * e){
	if(this->first == e && this->last==e){
		return true;
	}
	return false;
}

//updates first last pointers if necessary
//Klass contains at least 2 elem
void Klass::updateFirstLast(Elem * e){
	assert(this->first != this->last);
	if(this->first == e){
		this->first = this->first->getNext();
	}
	if(this->last == e){
		this->last = this->last->getPrev();
	}
}

/* inserts Klass before current Klass
 * the Klass k inserted contains at least one element
 */
void Klass::insertKlassBefore(Klass * k){
	if(this->prev != NULL){
		this->prev->next = k;
		k->prev = this->prev;
		//update pointers in element list front
		this->prev->last->updateNext(k->first);
		k->first->updatePrev(this->prev->last);
	}
	k->next = this;
	this->prev = k;
	//update pointers in element list back
	this->first->updatePrev(k->last);
	k->last->updateNext(this->first);
}

/*
 * symmetric to insertKlassBefore
 */
void Klass::insertKlassAfter(Klass * k){
	if(this->next != NULL){
		this->next->prev = k;
		k->next = this->next;
		//update pointers in element list end 
		this->next->first->updatePrev(k->last);
		k->last->updateNext(this->next->first);
	}
	k->prev= this;
	this->next = k;
	//update pointers in element list front 
	this->last->updateNext(k->first);
	k->first->updatePrev(this->last);
}

void Klass::insertElemEnd(Elem * e){
	this->last->insertAfter(e);
	this->last = e;
}

//splits if necessary
//return Klass in which we insert
Klass* Klass::insertElemBeforeSplit(int stamp, Elem * e){

	//case where current Klass contains one element
	//and new klass not created -> just update
	if(this->checkOnly(e) && (this->prev == NULL ||
													this->prev->getStamp() != stamp)){
		this->setStamp(stamp);
		return this;
	}

	//case where current Klass contains one element
	//and new klass created just move element
	if(this->checkOnly(e)){
		this->prev->setLast(e);
		Klass * prev = this->prev;
		e->setParent(prev);
		this->remove();
		return prev;
	}

	this->updateFirstLast(e);
	e->remove();

	// create new Klass if necessary
	// current Klass contains at least 2 elements
	if(this->prev == NULL || this->prev->getStamp() != stamp){
		Klass * k = new Klass(stamp,e);
		this->insertKlassBefore(k);
	//most general case new Klass created
	//just move element
	}else{
		this->prev->insertElemEnd(e);	
	}

	//this->prev->print(this->prev);
	e->setParent(this->prev);
	return this->prev;
}

//symetric to insertElemBeforeSplit
Klass* Klass::insertElemAfterSplit(int stamp, Elem * e){

	//case where current Klass contains one element
	//and new klass not created -> just update
	if(this->checkOnly(e) && (this->next == NULL ||
													this->next->getStamp() != stamp)){
		this->setStamp(stamp);
		return this;
	}

	//case where current Klass contains one element
	//and new klass created move element to end of new klass
	if(this->checkOnly(e)){
		e->remove();
		this->next->insertElemEnd(e);
		Klass * next = this->next;
		e->setParent(next);
		this->remove();
		return next;
	}

	this->updateFirstLast(e);
	e->remove();

	// create new Klass if necessary
	// current Klass contains at least 2 elements
	if(this->next == NULL || this->next->getStamp() != stamp){
		Klass * k = new Klass(stamp,e);
		this->insertKlassAfter(k);
	//most general case new Klass created
	//just move element
	}else{
		this->next->insertElemEnd(e);	
	}

	//this->prev->print(this->prev);
	e->setParent(this->next);
	return this->next;
}

