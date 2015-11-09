// PA193Project.cpp : Defines the entry point for the console application.
// author Martin Lastovicka
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

//no stack implementation, use this for testing
bool push(string element){return true;}
string pop(){return "";}
string top(){return "";}
bool isEmpty(){return true;}


/*
* Function to verify if element is a "void element", see http://www.w3.org/html/wg/drafts/html/master/syntax.html#elements-2
*
* return true   if element is void
*        false  if element is not void
*/
bool isVoidElement(string element) {
	string voidElements[] = { "area", "base", "br", "col", "embed", "hr", "img", "input", "keygen", "link",
		"menuitem", "meta", "param", "source", "track", "wbr" };
	for (int i = 0; i < 16; i++) {
		if (element.compare(voidElements[i]) == 0) {
			return true;
		}
	}
	return false;
}

int main(int argc, char * argv[]){
	if (argc != 2) {
		cerr << "You need to specify file to open" << "\n";
		return 0;
	}
	ifstream inStream(argv[1]);
	if (!inStream.is_open())
	{
		cerr << "ERROR: Unable to open file!" << "\n";
		return 0;
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
				break;
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
			if (current == '/') {
				state = END_ELEMENT;
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
				for (unsigned int i = 0; i < element.length(); i++) element[i] = (char)tolower(element[i]);
				if (element.compare("doctype") != 0) {
					cerr << "ERROR: invalid special element!" << "\n";
					invalidInput = true;
					break;
				} else {
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
			// we dont validate special tags attributes
			if (current == '>') {
				state = INSIDE_ELEMENT;
			}
			break;
		case INSIDE_COMMENT:
			if (current == '-' && previous == '-') {
				// only "-->" is valid closing sequence of a HTML comment
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
			//element name could end with " " (has attribute), "/" (self closing tag) or ">" (end of tag)
			if (current == ' ') {
				for (unsigned int i = 0; i < element.length(); i++) element[i] = (char) tolower(element[i]);
				if (!isValidElement(element)) {
					cerr << "ERROR: invalid element \"" << element << "\"" << "\n";
					invalidInput = true;
					break;
				}
				// "void elements" in HTML5 cannot be closed by closing tags
				if (!isVoidElement(element)) {
					if (!push(element)) {
						cerr << "ERROR: cannot push element to stack!" << "\n";
						invalidInput = true;
						break;
					}
				}				
				if(!addElement(element)) {
					cerr << "ERROR: cannot add element to list of elements!" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = PROCESS_ATTRIBUTE;
				break;
			}
			if (current == '/') {
				for (unsigned int i = 0; i < element.length(); i++) element[i] = (char)tolower(element[i]);
				if (!isValidElement(element)) {
					cerr << "ERROR: invalid element \"" << element << "\"" << "\n";
					invalidInput = true;
					break;
				}
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
				break;
			}
			if (current == '>') {
				for (unsigned int i = 0; i < element.length(); i++) element[i] = (char)tolower(element[i]);
				if (!isValidElement(element)) {
					cerr << "ERROR: invalid element \"" << element << "\"" << "\n";
					invalidInput = true;
					break;
				}
				// "void elements" in HTML5 cannot be closed by closing tags
				if (!isVoidElement(element)) {
					if (!push(element)) {
						cerr << "ERROR: cannot push element to stack!" << "\n";
						invalidInput = true;
						break;
					}
				}
				if (!addElement(element)) {
					cerr << "ERROR: cannot add element to list of elements!" << "\n";
					invalidInput = true;
					break;
				}
				element.clear();
				state = INSIDE_ELEMENT;
				break;
			}
			element += current;
			break;
		case PROCESS_ATTRIBUTE:
			if (current == '=' || current == ' ' || current == '>' || current == '/') {
				for (unsigned int i = 0; i < attribute.length(); i++) attribute[i] = (char)tolower(attribute[i]);
				if (!isValidAttribute(attribute)) {
					cerr << "ERROR: invalid attribute \"" << attribute << "\"" << "\n";
					invalidInput = true;
					break;
				}
				string currentElement = top();
				if (currentElement.empty()) {
					cerr << "ERROR: stack is empty while processing attribute" << "\n";
					//invalidInput = true;
					//break;
				}
				if (!isValidPair(currentElement, attribute)) {
					cerr << "ERROR: invalid pair of element and attribute: \"" << element << "\" - \"" << attribute << "\"" << "\n";
					invalidInput = true;
					break;
				}
				attribute.clear();
				if (current == '=') { state = DROP_ATTRIBUTE_VALUE; }
				// some attributes does not have to have value
				if (current == ' ') { state = PROCESS_ATTRIBUTE; }
				if (current == '>') { state = INSIDE_ELEMENT; }
				if (current == '/') { state = NEXT_ATTRIBUTE; }
				break;
			}
			if (attribute.compare("") == 0 && (current == ' ' || current == '\t' || current == '\n' || current == '\f' 
												|| current == '\r' || current == '\v')) {
				// whitespaces allowed before attribute
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
			if (current == '\"' && previous == '=') {
				state = DROP_ATTRIBUTE_VALUE_ESCAPED;
			}
			if (current == ' ') {
				state = PROCESS_ATTRIBUTE;
			}
			if (current == '>') {
				state = INSIDE_ELEMENT;
			}
			if (current == '/') {
				inStream.get(current);
				if (!inStream.good()) {
					cerr << "ERROR: an error occured while reading file!" << "\n";
					invalidInput = true;
					break;
				}
				if (current != '>') {
					cerr << "ERROR: invalid closing of tag with attribute" << "\n";
					invalidInput = true;
					break;
				}
				state = INSIDE_ELEMENT;
			}
			break;
		case DROP_ATTRIBUTE_VALUE_ESCAPED:
			if (current == '"' && previous != '\\') {
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
					cerr << "ERROR: invalid closing of tag with attribute" << "\n";
					invalidInput = true;
					break;
				}
				state = INSIDE_ELEMENT;
				break;
			}
			if (current == '>') {
				state = INSIDE_ELEMENT;
				break;
			}
			// this state just handles unexpected tag end after attribute
			state = PROCESS_ATTRIBUTE;
			break;
		case END_ELEMENT:
			if (current == '>') {
				for (unsigned int i = 0; i < element.length(); i++) element[i] = (char)tolower(element[i]);
				if (isEmpty()) {
					cerr << "ERROR: Too many element closings!" << "\n";
					//invalidInput = true;
					//break;
				}
				if (element.compare(top()) != 0) {
					cerr << "ERROR: invalid closing of element \"" << top() << "\", current closing is for \"" << element << "\"\n";
					//invalidInput = true;
					//break;
				}
				element.clear();
				pop();
				state = INSIDE_ELEMENT;
				break;
			}
			element += current;
			break;
		default:
			cerr << "ERROR: unexpected error occured!" << "\n";
			invalidInput = true;
			break;
		}

		previous = current;

		if (element.length() > 11) {
			cerr << "ERROR: invalid element occured!" << "\n";
			invalidInput = true;
			break;
		}

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


