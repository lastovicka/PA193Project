#pragma once
#include <iostream>
#include <string>

using namespace std;

/*
* Function that prints all used elements to standart output.
*/
void printAllElements();

/*
* Function to add element into list of all used elements. Does nothing if element is already present.
*
* return true   if element was successfuly added
*        false  if any error occurs
*/
bool addElement(string element);