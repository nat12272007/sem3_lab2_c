#include "doubly_list.h"

DoublyList::DoublyList() : head(nullptr), tail(nullptr), count(0) {}
DoublyList::~DoublyList() { clear(); }

void DoublyList::pushFront(const string& value) {
	LNode* node = new LNode(value, nullptr, head);
	if (head) head->prev = node;
	else tail = node;
	head = node;
	++count;
}

void DoublyList::pushBack(const string& value) {
	LNode* node = new LNode(value, tail, nullptr);
	if (tail) tail->next = node;
	else head = node;
	tail = node;
	++count;
}

void DoublyList::insertAfter(LNode* pos, const string& value) {
	if (!pos) return;
	LNode* node = new LNode(value, pos, pos->next);
	if (pos->next) pos->next->prev = node;
	else tail = node;
	pos->next = node;
	++count;
}

void DoublyList::insertBefore(LNode* pos, const string& value) {
	if (!pos) return;
	LNode* node = new LNode(value, pos->prev, pos);
	if (pos->prev) pos->prev->next = node;
	else head = node;
	pos->prev = node;
	++count;
}

bool DoublyList::insertAfterValue(const string& target, const string& value) {
	LNode* n = find(target);
	if (!n) return false;
	insertAfter(n, value);
	return true;
}

bool DoublyList::insertBeforeValue(const string& target, const string& value) {
	LNode* n = find(target);
	if (!n) return false;
	insertBefore(n, value);
	return true;
}

void DoublyList::popFront() {
	if (!head) return;
	LNode* tmp = head;
	head = head->next;
	if (head) head->prev = nullptr;
	else tail = nullptr;
	delete tmp;
	--count;
}

void DoublyList::popBack() {
	if (!tail) return;
	LNode* tmp = tail;
	tail = tail->prev;
	if (tail) tail->next = nullptr;
	else head = nullptr;
	delete tmp;
	--count;
}

void DoublyList::removeNode(LNode* node) {
	if (!node) return;
	if (node->prev) node->prev->next = node->next;
	else head = node->next;
	if (node->next) node->next->prev = node->prev;
	else tail = node->prev;
	delete node;
	--count;
}

bool DoublyList::removeAfter(const string& value) {
	LNode* target = find(value);
	if (target && target->next) { removeNode(target->next); return true; }
	return false;
}

bool DoublyList::removeBefore(const string& value) {
	LNode* target = find(value);
	if (target && target->prev) { removeNode(target->prev); return true; }
	return false;
}

LNode* DoublyList::find(const string& value) {
	for (LNode* cur = head; cur; cur = cur->next)
		if (cur->value == value) return cur;
	return nullptr;
}

bool DoublyList::contains(const string& value) { return find(value) != nullptr; }

bool DoublyList::removeByValue(const string& value) {
	LNode* node = find(value);
	if (node) { removeNode(node); return true; }
	return false;
}

string DoublyList::getByIndex(int index) {
	if (index < 0) return "ERROR";
	LNode* cur = head;
	for (int i = 0; cur && i < index; ++i) cur = cur->next;
	return cur ? cur->value : "ERROR";
}

void DoublyList::print() const {
	cout << "[";
	LNode* cur = head;
	bool first = true;
	while (cur) {
		if (!first) cout << " <-> ";
		cout << cur->value;
		cur = cur->next;
		first = false;
	}
	cout << "]\n";
}

void DoublyList::printBack() const {
	cout << "[";
	LNode* cur = tail;
	bool first = true;
	while (cur) {
		if (!first) cout << " <-> ";
		cout << cur->value;
		cur = cur->prev;
		first = false;
	}
	cout << "]\n";
}

void DoublyList::clear() { while (head) popFront(); }

bool handleDoublyListCommand(DoublyList& dl, const string& cmd, istringstream& iss) {
	string name, t, v; int i;
	if (cmd == "LADDHEAD") { iss >> name >> v; dl.pushFront(v); return true; }
	if (cmd == "LADDTAIL") { iss >> name >> v; dl.pushBack(v); return true; }
	if (cmd == "LADDAFTER") { iss >> name >> t >> v; dl.insertAfterValue(t, v); return true; }
	if (cmd == "LADDBEFORE") { iss >> name >> t >> v; dl.insertBeforeValue(t, v); return true; }
	if (cmd == "LDELHEAD") { iss >> name; dl.popFront(); return true; }
	if (cmd == "LDELTAIL") { iss >> name; dl.popBack(); return true; }
	if (cmd == "LDELAFTER") { iss >> name >> t; dl.removeAfter(t); return true; }
	if (cmd == "LDELBEFORE") { iss >> name >> t; dl.removeBefore(t); return true; }
	if (cmd == "LDELVAL") { iss >> name >> v; dl.removeByValue(v); return true; }
	if (cmd == "LGET") { iss >> name >> i; cout << dl.getByIndex(i) << "\n"; return true; }
	if (cmd == "LFIND") { iss >> name >> v; cout << (dl.contains(v) ? "TRUE" : "FALSE") << "\n"; return true; }
	if (cmd == "LPRINTBACK") { iss >> name; dl.printBack(); return true; }
	return false;
}
