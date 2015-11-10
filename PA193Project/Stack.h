#pragma once
#include <string>
#include <iostream>
using namespace std;

const int stack_size = 550;

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
	string topOfStack();
};


