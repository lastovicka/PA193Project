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
	string element = "";
	string attribute = "";
	int state = START;
	while (inStream.get(current))
	{
		if (!inStream.good()) {
			cerr << "ERROR: an error occured while reading file!" << "\n";
			invalidInput = true;
			break;
		}

		switch (state)
		{
		case START:
			if (current != '<' && current != ' ' && current != '\t' && current != '\n' && current != '\f' && current != '\r' && current != '\v') {
				// only whitespaces allowed before first element
				invalidInput = true;
			}
			if (current == '<'){
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
				element.clear();
				state = INSIDE_COMMENT;
				break;
			}
			if (current == ' ') {
				// DOCTYPE is the only special element allowed
				if (element.compare("DOCTYPE") != 0) {
					cerr << "ERROR: invalid special element!" << "\n";
					invalidInput = true;
					break;
				}
				else {
					if (!addElement(element)) {
						cerr << "ERROR: cannot add element to list of elements!" << "\n";
						invalidInput = true;
						break;
					}
					element.clear();
					state = DROP_SPECIAL_TAG;
					break;
				}
			}
			element += current;
			break;
		case DROP_SPECIAL_TAG:
			if (current == '>') {
				state = INSIDE_ELEMENT;
			}
			break;
		case INSIDE_COMMENT:
			if (current == '-' && previous == '-') {
				inStream.get(current);
				if (!inStream.good()) {
					cerr << "ERROR: an error occured while reading file!" << "\n";
					invalidInput = true;
					break;
				}
				if (current != '>') {
					cerr << "ERROR: invalid closing of comment" << "\n";
					invalidInput = true;
					break;
				}
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
				if(!addElement(element)) {
					cerr << "ERROR: cannot add element to list of elements!" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = PROCESS_ATTRIBUTE;
			}
			if (current == '/') {
				// dont push element on stack, it closed itself

				if (!addElement(element)) {
					cerr << "ERROR: cannot add element to list of elements!" << "\n";
					invalidInput = true;
					break;
				}
				inStream.get(current);
				if (!inStream.good()) {
					cerr << "ERROR: an error occured while reading file!" << "\n";
					invalidInput = true;
					break;
				}
				if (current != '>') {
					cerr << "ERROR: invalid closing of comment" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = INSIDE_ELEMENT;
			}
			if (current == '>') {
				if (!push(element)) {
					cerr << "ERROR: cannot push element to stack!" << "\n";
					invalidInput = true;
					break;
				}
				if (!addElement(element)) {
					cerr << "ERROR: cannot add element to list of elements!" << "\n";
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
				attribute.clear();
				state = DROP_ATTRIBUTE_VALUE;
				break;
			}
			if (current == ' ' || current == '\t' || current == '\n' || current == '\f' || current == '\r' || current == '\v') {
				// whitespaces allowed
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
				inStream.get(current);
				if (!inStream.good()) {
					cerr << "ERROR: an error occured while reading file!" << "\n";
					invalidInput = true;
					break;
				}
				if (current != '>') {
					cerr << "ERROR: invalid closing of comment" << "\n";
					invalidInput = true;
					break;
				}
				state = INSIDE_ELEMENT;
			}
			if (current == '>') {
				state = INSIDE_ELEMENT;
			}
			if (current == ' ' || current == '\t' || current == '\n' || current == '\f' || current == '\r' || current == '\v') {
				state = PROCESS_ATTRIBUTE;
			}
			break;
		case END_ELEMENT:
			if (current == '>') {
				if (isEmpty()) {
					cerr << "ERROR: Too many element closings!" << "\n";
					invalidInput = true;
					break;
				}
				if (element.compare(top()) != 0) {
					cerr << "ERROR: invalid closing of element " << top() << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				pop();
				state = INSIDE_ELEMENT;
			}
			element += current;
			break;
		default:
			cerr << "ERROR: unexpected error occured!" << "\n";
			invalidInput = true;
			break;
		}

		previous = current;

		if (invalidInput) {
			cerr << "ERROR: Validation of HTML file failed!" << "\n";
			break;
		}
		
	}
	inStream.close();

	if (!invalidInput && !element.empty()) {
		cerr << "ERROR: Some element was not closed correctly!" << "\n";
		invalidInput = true;
	}

	if (!invalidInput && !attribute.empty()) {
		cerr << "ERROR: Some attribute was not closed correctly!" << "\n";
		invalidInput = true;
	}

	if (!invalidInput && !isEmpty()) {
		cerr << "ERROR: Not all elements were closed!" << "\n";
		invalidInput = true;
	}

	// parsing was succesfull, print output
	if (!invalidInput) {
		printAllElements();
	}

	return 0;
}

