#pragma once

#include "List1.h"

template <class Item>
class Stack1 {
 public:
     Stack1( );    // constructor for empty stack
     Stack1(unsigned int capacity); // construct a stack with
								   // initial capacity of capacity
     ~Stack1( );  // destructor
     bool isEmpty( ) const;
     // returns true if stack is empty, and false otherwise   cont’d..
bool isFull( ) const;
     // returns true if stack is full, and false otherwise 
     void push( Item k );
     // put k on top of the stack
     Item pop( );
     // Remove and return the top value from the stack; if the 
     // stack is empty, return NULL instead
     Item peek( ) const;
     // return the top value without modifying the stack; if the 
     // stack is empty, return NULL instead  

	 unsigned int getSize() const {return m_size;}
private:
     unsigned int  m_size;
     List<Item> * m_container;
     
};
#include "Stack.template"
