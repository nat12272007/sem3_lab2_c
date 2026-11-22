#include "set.h"

Set::Set() : count(0) {}

bool Set::add(const string& value) {
	if (list.contains(value)) return false;
	list.pushBack(value);
	++count;
	return true;
}

bool Set::remove(const string& value) {
	if (list.removeByValue(value)) { --count; return true; }
	return false;
}

bool Set::contains(const string& value) const {
	ForwardList& l = const_cast<ForwardList&>(list);
	return l.contains(value);
}

vector<string> Set::getAll() const {
	vector<string> out;
	FNode* cur = list.getHead();
	while (cur) { out.push_back(cur->value); cur = cur->next; }
	return out;
}

void Set::clear() { list.clear(); count = 0; }

bool handleSetCommand(Set& s, const string& cmd, istringstream& iss) {
	string v;
	if (cmd == "SETADD") {
		if (!(iss >> v)) return true;
		cout << (s.add(v) ? "TRUE" : "FALSE") << "\n";
		return true;
	}
	if (cmd == "SETDEL") {
		if (!(iss >> v)) return true;
		cout << (s.remove(v) ? "TRUE" : "FALSE") << "\n";
		return true;
	}
	if (cmd == "SET_AT") {
		if (!(iss >> v)) return true;
		cout << (s.contains(v) ? "TRUE" : "FALSE") << "\n";
		return true;
	}
	if (cmd == "SETPRINT") {
		vector<string> elems = s.getAll();
		cout << "{ ";
		for (const string& e : elems) cout << e << " ";
		cout << "}\n";
		return true;
	}
	return false;
}
