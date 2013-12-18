#pragma once

#import <cstdlib>

const int MAX_LIST = 200;


template <class Item>
class List{
public: 
    List();  // constructor for empty list with default capacity
    List(unsigned int capacity); // constructor for empty list that can hold up to capacity items
    ~List(); // destructor

    bool isEmpty() const; // returns true if the list is empty, and false otherwise  
	bool isFull() const; // returns true if the list is full, and false otherwise 
    unsigned int getLength() const; // returns the number of items currently in the list
    
	bool insert (unsigned int pos, Item item); // inserts item into the list at location pos; returns true if 
										       // operation succeeds, and false otherwise
	Item remove (unsigned int pos); // removes the item at location pos from the list, and returns it; 
                                    // if operation fails, returns NULL instead                           
	Item retrieve (unsigned int pos) const; // find the value stored at location pos in the list, and return it; 
	                                        // if the operation fails, return NULL instead
	Item replace (unsigned int pos, Item item); // replaces the element stored at position pos by the new item
												// returns the displaced item; if pos is not a valid location 
	                                            // or is the position immediately beyond the list’s end 
	                                            // it returns NULL instead. 
	
	bool append (Item item); // adds an item at the end of the list
	inline Item removeLast()   {return remove(getLength());}   // removes the last item in the list, if any, returns NULL otherwise 
	inline Item retrieveLast() const {return retrieve(getLength());} // retreive the last item in the list, if any, returns NULL otherwise 

private:
    Item * m_container; // pointer to array to hold the list
    unsigned int m_max_capacity; // maximum capacity of the list
    unsigned int m_size;    // number of items currently in the list
    unsigned int translate(unsigned int pos) const; // converts a list position to the corresponding array index
	void replaceContainer(); // private function replaces the underlying array with one that holds
							 // the same values as the original, but has double the capacity

};

#include "List.template" // must include implementation
