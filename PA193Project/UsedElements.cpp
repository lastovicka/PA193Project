#include "stdafx.h"
#include "UsedElements.h"

string usedElements[116];
int top = -1;

void printAllElements() {
	cout << "List of all elements used in parsed file:" << "\n";
	for (int i = 0; i <= top; i++) {
		cout << usedElements[i] << "\n";
	}
}

bool addElement(string element) {
	if (top == 115) {
		cerr << "ERROR: You used more elements than HTML allows!\n";
		return false;
	}
	for (int i = 0; i <= top; i++) {
		if (element.compare(usedElements[i]) == 0) {
			return true;
		}
	}	
	top++;
	usedElements[top] = element;
	return true;
}