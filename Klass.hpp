#ifndef KLASS_HPP_INCLUDED
#define KLASS_HPP_INCLUDED

#include "Elem.hpp"

class Klass {
	int size;
	int stamp;
	Klass * prev,* next;
	Elem * first,* last;

	public:

	//debug function
  void print(Klass * par);
	Klass(int stamp, Elem * e);
	int getStamp();
	void setStamp(int stamp);

	bool checkPrevSameStamp();
	bool checkNextSameStamp();

	void setLast(Elem * e);
	Elem * getFirst();
	Klass * getPrev();
	Klass * getNext();
	void remove();

	//checks if e is the only element in Klass
	bool checkOnly(Elem * e);
					

	//updates first last pointers if necessary
	//Klass contains at least 2 elem
	void updateFirstLast(Elem * e);
	/* inserts Klass before current Klass
	 * the Klass k inserted contains at least one element
	 */
	void insertKlassBefore(Klass * k);
	void insertKlassAfter(Klass * k);
	void insertElemEnd(Elem * e);

	//splits if necessary
	//return Klass in which we insert
	Klass* insertElemBeforeSplit(int stamp, Elem * e);
	Klass* insertElemAfterSplit(int stamp, Elem * e);

};

#endif // KLASS_HPP_INCLUDED
