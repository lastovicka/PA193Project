// PA193Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "AttributeChecker.h"
#include "ElementChecker.h"
#include "Stack.h"
#include "States.h"
#include "UsedElements.h"

using namespace std;

int main()
{
	ifstream inStream("example.htm");
	if (!inStream.is_open())
	{
		cerr << "ERROR: Unable to open file!" << "\n";
	}

	bool invalidInput = false;
	char current;
	char previous = '\0';
	string element;
	string attribute;
	int state = START;
	while (inStream.get(current))
	{
		if (!inStream.good()) {
			cerr << "ERROR: an error occured while reading file!" << "\n";
			break;
		}

		switch (state)
		{
		case START:
			if (current != '<') {
				invalidInput = true;
			}
			else {
				state = INSIDE_TAG;
			}
			break;
		case INSIDE_TAG:
			if (current == '!') {
				state = INSIDE_SPECIAL_TAG;
				break;
			}
			element += current;
			state = PROCESS_ELEMENT;
			break;
		case INSIDE_SPECIAL_TAG:
			if (current == '-' && previous == '-') {
				state = INSIDE_COMMENT;
				break;
			}
			// TODO: handle special tags
			break;
		case INSIDE_COMMENT:
			if (current == '-' && previous == '-') {
				//TODO: drop >
				state = INSIDE_ELEMENT;
				break;
			}
			// do nothing, we dont care what is inside comments
			break;
		case PROCESS_ELEMENT:
			if (current == ' ') {
				if (!push(element)) {
					cerr << "ERROR: cannot push element to stack!" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = PROCESS_ATTRIBUTE;
			}
			if (current == '/') {
				if (!push(element)) {
					cerr << "ERROR: cannot push element to stack!" << "\n";
					invalidInput = true;
					break;
				}
				//TODO: drop >
				element.clear();
				state = INSIDE_ELEMENT;
			}
			if (current == '>') {
				if (!push(element)) {
					cerr << "ERROR: cannot push element to stack!" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = INSIDE_ELEMENT;
			}
			element += current;
			break;
		case PROCESS_ATTRIBUTE:
			if (current == '=') {
				if (!isValidAttribute(attribute)) {
					cerr << "ERROR: invalid attribute \"" << attribute << "\"" << "\n";
					invalidInput = true;
					break;
				}
				string currentElement = top();
				if (currentElement.empty()) {
					cerr << "ERROR: stack is empty while processing attribute" << "\n";
					invalidInput = true;
					break;
				}
				if (!isValidPair(currentElement, attribute)) {
					cerr << "ERROR: invalid pair of element and attribute: \"" << element << "\" - \"" << attribute << "\"" << "\n";
					invalidInput = true;
					break;
				}
				state = DROP_ATTRIBUTE_VALUE;
				break;
			}
			attribute += current;
			break;
		case INSIDE_ELEMENT:
			if (current == '<') {
				state = INSIDE_TAG;
			}
			// do nothing, we dont care what is element content
			break;
		case DROP_ATTRIBUTE_VALUE:
			if (current == '"' && previous != '=') {
				state = NEXT_ATTRIBUTE;
			}
			break;
		case NEXT_ATTRIBUTE:
			if (current == '/') {
				//TODO: drop >
				state = INSIDE_ELEMENT;
			}
			if (current == '>') {
				state = INSIDE_ELEMENT;
			}
			if (current == ' ') {
				state = PROCESS_ATTRIBUTE;
			}
			break;
		default:
			cerr << "ERROR: unexpected error occured!" << "\n";
			invalidInput = true;
			break;
		}

		previous = current;

		if (invalidInput) {
			cerr << "Validation of HTML file failed!" << "\n";
			break;
		}
		
	}
	inStream.close();

	// parsing was succesfull, print output
	if (!invalidInput) {
		printAllElements();
	}

	return 0;
}

