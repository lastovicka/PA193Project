#pragma once
#include <iostream>
#include <string>

using namespace std;

/*
* Function that prints all used attributes to standart output.
*/
void printAllAttributes();

/*
* Function to add attribute into list of all used attributes. Does nothing if attribute is already present.
*
* return true   if attribute was successfuly added
*        false  if any error occurs
*/
bool addAttribute(string attribute);