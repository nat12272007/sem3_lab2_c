#include "forward_list.h"

ForwardList::ForwardList() : head(nullptr), tail(nullptr), count(0) {}
ForwardList::~ForwardList() { clear(); }

void ForwardList::pushFront(const string& value) {
	FNode* node = new FNode(value, head);
	head = node;
	if (!tail) tail = node;
	++count;
}

void ForwardList::pushBack(const string& value) {
	FNode* node = new FNode(value, nullptr);
	if (tail) tail->next = node;
	else head = node;
	tail = node;
	++count;
}

void ForwardList::insertAfter(FNode* prev, const string& value) {
	if (!prev) return;
	FNode* node = new FNode(value, prev->next);
	prev->next = node;
	if (prev == tail) tail = node;
	++count;
}

void ForwardList::insertBefore(FNode* target, const string& value) {
	if (head == target) { pushFront(value); return; }
	FNode* prev = head;
	while (prev && prev->next != target) prev = prev->next;
	if (prev) insertAfter(prev, value);
}

bool ForwardList::insertAfterValue(const string& target, const string& value) {
	FNode* n = find(target);
	if (!n) return false;
	insertAfter(n, value);
	return true;
}

bool ForwardList::insertBeforeValue(const string& target, const string& value) {
	FNode* n = find(target);
	if (!n) return false;
	insertBefore(n, value);
	return true;
}

void ForwardList::popFront() {
	if (!head) return;
	FNode* tmp = head;
	head = head->next;
	if (!head) tail = nullptr;
	delete tmp;
	--count;
}

void ForwardList::popBack() {
	if (!head) return;
	if (head == tail) { delete head; head = tail = nullptr; --count; return; }
	FNode* prev = head;
	while (prev->next != tail) prev = prev->next;
	delete tail;
	tail = prev;
	tail->next = nullptr;
	--count;
}

void ForwardList::removeAfter(FNode* prev) {
	if (!prev || !prev->next) return;
	FNode* toDelete = prev->next;
	prev->next = toDelete->next;
	if (toDelete == tail) tail = prev;
	delete toDelete;
	--count;
}

void ForwardList::removeBefore(FNode* target) {
	if (!head || head == target) return;
	if (head->next == target) { popFront(); return; }
	FNode* prev = head;
	while (prev->next && prev->next->next != target) prev = prev->next;
	if (prev->next) removeAfter(prev);
}

bool ForwardList::removeAfterValue(const string& target) {
	FNode* n = find(target);
	if (!n || !n->next) return false;
	removeAfter(n);
	return true;
}

bool ForwardList::removeBeforeValue(const string& target) {
	FNode* n = find(target);
	if (!n || n == head) return false;
	removeBefore(n);
	return true;
}

FNode* ForwardList::find(const string& value) {
	for (FNode* cur = head; cur; cur = cur->next)
		if (cur->value == value) return cur;
	return nullptr;
}

bool ForwardList::contains(const string& value) { return find(value) != nullptr; }

bool ForwardList::removeByValue(const string& value) {
	if (!head) return false;
	if (head->value == value) { popFront(); return true; }
	FNode* prev = head;
	while (prev->next && prev->next->value != value) prev = prev->next;
	if (prev->next) { removeAfter(prev); return true; }
	return false;
}

string ForwardList::getByIndex(int index) {
	if (index < 0) return "ERROR";
	FNode* cur = head;
	for (int i = 0; cur && i < index; ++i) cur = cur->next;
	return cur ? cur->value : "ERROR";
}

void ForwardList::print() const {
	cout << "[";
	FNode* cur = head;
	bool first = true;
	while (cur) {
		if (!first) cout << " -> ";
		cout << cur->value;
		cur = cur->next;
		first = false;
	}
	cout << "]\n";
}

void ForwardList::clear() { while (head) popFront(); }

bool handleForwardListCommand(ForwardList& fl, const string& cmd, istringstream& iss) {
	string name, t, v; int i;
	if (cmd == "FADDHEAD") { iss >> name >> v; fl.pushFront(v); return true; }
	if (cmd == "FADDTAIL") { iss >> name >> v; fl.pushBack(v); return true; }
	if (cmd == "FADDAFTER") { iss >> name >> t >> v; fl.insertAfterValue(t, v); return true; }
	if (cmd == "FADDBEFORE") { iss >> name >> t >> v; fl.insertBeforeValue(t, v); return true; }
	if (cmd == "FDELHEAD") { iss >> name; fl.popFront(); return true; }
	if (cmd == "FDELTAIL") { iss >> name; fl.popBack(); return true; }
	if (cmd == "FDELAFTER") { iss >> name >> t; fl.removeAfterValue(t); return true; }
	if (cmd == "FDELBEFORE") { iss >> name >> t; fl.removeBeforeValue(t); return true; }
	if (cmd == "FDELVAL") { iss >> name >> v; fl.removeByValue(v); return true; }
	if (cmd == "FGET") { iss >> name >> i; cout << fl.getByIndex(i) << "\n"; return true; }
	if (cmd == "FFIND") { iss >> name >> v; cout << (fl.contains(v) ? "TRUE" : "FALSE") << "\n"; return true; }
	return false;
}
