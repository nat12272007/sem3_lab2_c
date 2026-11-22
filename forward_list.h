#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct FNode {
	string value;
	FNode* next;
	FNode(const string& v, FNode* n = nullptr) : value(v), next(n) {}
};

struct ForwardList {
	FNode* head;
	FNode* tail;
	int count;

	ForwardList();
	~ForwardList();

	void pushFront(const string& value);
	void pushBack(const string& value);
	void insertAfter(FNode* prev, const string& value);
	void insertBefore(FNode* target, const string& value);
	bool insertAfterValue(const string& target, const string& value);
	bool insertBeforeValue(const string& target, const string& value);

	void popFront();
	void popBack();
	void removeAfter(FNode* prev);
	void removeBefore(FNode* target);
	bool removeAfterValue(const string& target);
	bool removeBeforeValue(const string& target);

	FNode* find(const string& value);
	bool contains(const string& value);
	bool removeByValue(const string& value);
	string getByIndex(int index);

	void print() const;
	void clear();

	int getCount() const { return count; }
	FNode* getHead() const { return head; }
};

bool handleForwardListCommand(ForwardList& fl, const string& cmd, istringstream& iss);

#endif
