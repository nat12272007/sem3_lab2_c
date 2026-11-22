#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Stack {
	string* data;
	int top;
	int capacity;

	Stack(int cap = 4);
	~Stack();

	void push(const string& value);
	bool pop();
	bool popValue(string& out);
	string peek() const;
	void print() const;
	void clear();

	int getTop() const { return top; }
	string getElement(int index) const { return data[index]; }
};

bool handleStackCommand(Stack& stk, const string& cmd, istringstream& iss);

#endif
