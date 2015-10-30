#pragma once
#include <string>

using namespace std;

/*
* Function to verify if attribute is a valid HTML5 attribute.
*
* return true   if attribute is valid
*        false  if attribute is not valid
*/
bool isValidAttribute(string attribute);

/*
* Function to verify if given attribute can be used with specified element.
*
* return true   if element - attribute pair is valid
*        false  if element - attribute pair is not valid
*/
bool isValidPair(string element, string attribute);

