#include "Elem.hpp"

#include <cstdlib>

using namespace std;

Elem::Elem(int id, Klass * parent){
	this->id = id;
	this->parent = parent;
	prev = next = NULL;
}

Klass * Elem::getParent(){
	return parent;
}

void Elem::setParent(Klass * parent){
	this->parent = parent;
}

void Elem::updatePrev(Elem * e){
	this->prev = e;
}

void Elem::updateNext(Elem * e){
	this->next = e;
}

Elem * Elem::getNext(){
	return this->next;
}

Elem * Elem::getPrev(){
	return this->prev;
}

int Elem::getId(){
	return id;
}

void Elem::insertBefore(Elem * e){
	if(this->prev != NULL){
		this->prev->next = e;
		e->prev = this->prev;
	}
	e->next = this;
	this->prev = e;
}

void Elem::insertAfter(Elem * e){
	if(this->next != NULL){
		this->next->prev = e;
		e->next = this->next ;
	}
	e->prev = this;
	this->next = e;
}

void Elem::remove(){
	if(this->prev != NULL){
		this->prev->next = this->next;
	}
	if(this->next != NULL){
		this->next->prev = this->prev;
	}
	this->prev = this->next = NULL;
}

