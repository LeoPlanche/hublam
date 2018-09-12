#ifndef ELEM_HPP_INCLUDED
#define ELEM_HPP_INCLUDED

class Klass;

class Elem{
	int id;
	Klass * parent;
	Elem * prev, *next;
	
	public:

	Elem(int id, Klass * parent);
	Klass * getParent();
	void setParent(Klass * parent);
	void updatePrev(Elem * e);
	void updateNext(Elem * e);
	Elem * getNext();
	Elem * getPrev();
	int getId();
	void insertBefore(Elem * e);
	void insertAfter(Elem * e);
	void remove();
};

#endif // ELEM_HPP_INCLUDED
