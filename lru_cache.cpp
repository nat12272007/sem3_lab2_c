#include "lru_cache.h"
#include "doubly_list.h"

struct LRUState {
	int capacity;
	DoublyList cache;
	LRUState() : capacity(2) {}

	LNode* findByKey(int key) {
		for (LNode* cur = cache.getHead(); cur; cur = cur->next) {
			istringstream iss(cur->value);
			int k; char colon;
			if ((iss >> k >> colon) && k == key) return cur;
		}
		return nullptr;
	}
};

static LRUState lru;

static int extractValue(const string& s) {
	size_t pos = s.find(':');
	return stoi(s.substr(pos + 1));
}

bool handleLRUCommand(const string& cmd, istringstream& iss) {
	if (cmd == "LRU_INIT") {
		int cap; if (!(iss >> cap)) return true;
		lru.cache.clear();
		lru.capacity = cap;
		return true;
	}
	if (cmd == "LRU_SET") {
		int k, v; if (!(iss >> k >> v)) return true;
		LNode* n = lru.findByKey(k);
		if (n) lru.cache.removeNode(n);
		else if (lru.cache.getCount() >= lru.capacity) lru.cache.popBack();
		lru.cache.pushFront(to_string(k) + ":" + to_string(v));
		return true;
	}
	if (cmd == "LRU_GET") {
		int k; if (!(iss >> k)) return true;
		LNode* n = lru.findByKey(k);
		if (!n) { cout << "-1\n"; return true; }
		int val = extractValue(n->value);
		lru.cache.removeNode(n);
		lru.cache.pushFront(to_string(k) + ":" + to_string(val));
		cout << val << "\n";
		return true;
	}
	if (cmd == "LRU_PRINT") {
		cout << "LRU [";
		LNode* cur = lru.cache.getHead();
		bool first = true;
		while (cur) {
			if (!first) cout << ", ";
			cout << cur->value;
			cur = cur->next;
			first = false;
		}
		cout << "]\n";
		return true;
	}
	return false;
}

void saveLRUState(ostream& out) {
	out << "LRU " << lru.cache.getCount() << " " << lru.capacity << "\n";
	for (LNode* cur = lru.cache.getHead(); cur; cur = cur->next) {
		out << cur->value << " ";
	}
	out << "\n";
}

void loadLRUState(istream& in) {
	string type;
	int count, cap;
	if (!(in >> type >> count >> cap)) return;
	if (type == "LRU") {
		lru.cache.clear();
		lru.capacity = cap;
		for (int i = 0; i < count; ++i) {
			string kv;
			if (in >> kv) lru.cache.pushBack(kv);
		}
	}
}
