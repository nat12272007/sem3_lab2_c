#ifndef SET_H
#define SET_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "forward_list.h"

using namespace std;

struct Set {
	ForwardList list;
	int count;

	Set();
	bool add(const string& value);
	bool remove(const string& value);
	bool contains(const string& value) const;
	int size() const { return count; }
	vector<string> getAll() const;
	void clear();
};

bool handleSetCommand(Set& s, const string& cmd, istringstream& iss);

#endif
