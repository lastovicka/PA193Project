#include "stdafx.h"
#include "UsedElements.h"

string usedAttributes[119];
int topOfAttributes = -1;

void printAllAttributes() {
	cout << "List of all attributes used in parsed file:" << "\n";
	for (int i = 0; i <= topOfAttributes; i++) {
		cout << usedAttributes[i] << "\n";
	}
}

bool addAttribute(string attribute) {
	if (topOfAttributes == 118) {
		cerr << "ERROR: You used more attributes than HTML5 allows!\n";
		return false;
	}
	for (int i = 0; i <= topOfAttributes; i++) {
		if (attribute.compare(usedAttributes[i]) == 0) {
			return true;
		}
	}
	topOfAttributes++;
	usedAttributes[topOfAttributes] = attribute;
	return true;
}