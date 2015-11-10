#include "stdafx.h"
#include "Stack.h"

//=====
/*
bool push(string element) {
return true;
}

string pop() {
return "";
}

string top() {
return "";
}

bool isEmpty() {
return true;
}
*/
//=======
/*
const int stack_size = 1000;

class stack
{
private:
	// data for the stack
	string data[stack_size];
	// the index of the top element of the stack
	int top;
public:
	stack();   // constructor creates an empty stack
	bool push(string element);  // puts item on the top of the stack
	string pop();   // removes and returns the item at the top of stack
	bool isEmpty();
	bool full();
};*/

// initialize the stack to an empty stack
stack::stack()
{
	top = -1;
}

// full returns true if the stack is full, else it returns false
bool stack::full()
{
	return top == stack_size - 1;
}

// push adds a new element to the top of the stack
bool stack::push(string element)
{
	// if the stack is full, error
	if (full())
	{
		cerr << "\n\nStack Error: pushing on a full stack";
		return false;
	}
	else  // OK to add the element
	{
		top++;
		data[top] = element;
		return true;
	}
	
}

// empty returns true if the stack is empty, else it returns false
bool stack::isEmpty()
{
	return top == -1;
}

// pop removes and returns the top element of the stack
string stack::pop()
{
	// if the stack is empty, error
	if (isEmpty())
	{
		//cerr << "\n\nStack Error: Popping an empty stack";
		return "empty stack ?";
	}
	else // OK to pop
	{
		top--;
		return data[top + 1];
	}
}

string stack::topOfStack()
{
	// if the stack is empty, error
	if (isEmpty())
	{
		//cerr << "\n\nStack Error: Popping an empty stack";
		return "empty stack ?";
	}
	else // OK to pop
	{
		return data[top];
	}
}

