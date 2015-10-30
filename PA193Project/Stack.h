#pragma once
#include <string>

using namespace std;

/*
* Function to put new element on top of the stack.
*
* return true   if element was successfuly added
*        false  if any error occurs
*/
bool push(string element);

/*
* Function to remove element from top of the stack
*
* return element           element from top of the stack
*        empty  string     if any error occurs
*/
string pop();

/*
* Function to get element from top of the stack. Element is not removed from the stack.
*
* return element  element from top of the stack
*        empty  string     if any error occurs
*/
string top();

/*
* Function to verify is there is any element in stack.
*
* return true   if stack is empty
*        false  if stack is not
*/
bool isEmpty();