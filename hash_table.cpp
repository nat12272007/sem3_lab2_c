#include "hash_table.h"
#include <functional>

HashMapChaining::HashMapChaining() : size(0) {}

int HashMapChaining::hash(const string& key) const {
	return std::hash<string>{}(key) % NUM_BUCKETS;
}

void HashMapChaining::put(const string& key, const string& value) {
	int idx = hash(key);
	FNode* cur = buckets[idx].getHead();
	while (cur) {
		size_t pos = cur->value.find(':');
		string k = cur->value.substr(0, pos);
		if (k == key) {
			cur->value = key + ":" + value;
			return;
		}
		cur = cur->next;
	}
	buckets[idx].pushBack(key + ":" + value);
	++size;
}

bool HashMapChaining::get(const string& key, string& value) const {
	int idx = hash(key);
	ForwardList& fl = const_cast<ForwardList&>(buckets[idx]);
	for (FNode* cur = fl.getHead(); cur; cur = cur->next) {
		size_t pos = cur->value.find(':');
		string k = cur->value.substr(0, pos);
		if (k == key) { value = cur->value.substr(pos + 1); return true; }
	}
	return false;
}

bool HashMapChaining::remove(const string& key) {
	int idx = hash(key);
	FNode* cur = buckets[idx].getHead();
	FNode* prev = nullptr;
	while (cur) {
		size_t pos = cur->value.find(':');
		string k = cur->value.substr(0, pos);
		if (k == key) {
			if (prev) buckets[idx].removeAfter(prev);
			else buckets[idx].popFront();
			--size;
			return true;
		}
		prev = cur;
		cur = cur->next;
	}
	return false;
}

bool HashMapChaining::contains(const string& key) const { string d; return get(key, d); }

void HashMapChaining::clear() {
	for (int i = 0; i < NUM_BUCKETS; ++i) buckets[i].clear();
	size = 0;
}

void HashMapChaining::save(ostream& out) const {
	out << size << "\n";
	for (int i = 0; i < NUM_BUCKETS; ++i) {
		for (FNode* cur = buckets[i].getHead(); cur; cur = cur->next) {
			out << cur->value << " ";
		}
	}
	out << "\n";
}

void HashMapChaining::load(istream& in) {
	clear();
	int n;
	if (!(in >> n)) return;
	for (int i = 0; i < n; ++i) {
		string kv;
		if (in >> kv) {
			size_t pos = kv.find(':');
			string k = kv.substr(0, pos);
			string v = kv.substr(pos + 1);
			put(k, v);
		}
	}
}

HashMapCuckoo::HashMapCuckoo() : size(0) {
	for (int i = 0; i < TABLE_SIZE; ++i) { table1[i].used = false; table2[i].used = false; }
}

int HashMapCuckoo::hash1(const string& key) const {
	return std::hash<string>{}(key) % TABLE_SIZE;
}

int HashMapCuckoo::hash2(const string& key) const {
	return (std::hash<string>{}(key) / TABLE_SIZE + 1) % TABLE_SIZE;
}

void HashMapCuckoo::put(const string& key, const string& value) {
	string dummy;
	if (get(key, dummy)) {
		int idx = hash1(key);
		if (table1[idx].used && table1[idx].key == key) { table1[idx].value = value; return; }
		idx = hash2(key);
		if (table2[idx].used && table2[idx].key == key) { table2[idx].value = value; return; }
	}
	string curKey = key; string curVal = value;
	for (int loop = 0; loop < MAX_LOOP; ++loop) {
		int idx1 = hash1(curKey);
		if (!table1[idx1].used) {
			table1[idx1].key = curKey; table1[idx1].value = curVal; table1[idx1].used = true;
			++size; return;
		}
		swap(curKey, table1[idx1].key);
		swap(curVal, table1[idx1].value);
		int idx2 = hash2(curKey);
		if (!table2[idx2].used) {
			table2[idx2].key = curKey; table2[idx2].value = curVal; table2[idx2].used = true;
			++size; return;
		}
		swap(curKey, table2[idx2].key);
		swap(curVal, table2[idx2].value);
	}
	clear(); put(key, value);
}

bool HashMapCuckoo::get(const string& key, string& value) const {
	int i1 = hash1(key);
	if (table1[i1].used && table1[i1].key == key) { value = table1[i1].value; return true; }
	int i2 = hash2(key);
	if (table2[i2].used && table2[i2].key == key) { value = table2[i2].value; return true; }
	return false;
}

bool HashMapCuckoo::remove(const string& key) {
	int i1 = hash1(key);
	if (table1[i1].used && table1[i1].key == key) { table1[i1].used = false; --size; return true; }
	int i2 = hash2(key);
	if (table2[i2].used && table2[i2].key == key) { table2[i2].used = false; --size; return true; }
	return false;
}

bool HashMapCuckoo::contains(const string& key) const { string d; return get(key, d); }

void HashMapCuckoo::clear() {
	for (int i = 0; i < TABLE_SIZE; ++i) { table1[i].used = false; table2[i].used = false; }
	size = 0;
}

void HashMapCuckoo::save(ostream& out) const {
	out << size << "\n";
	for (int i = 0; i < TABLE_SIZE; ++i) {
		if (table1[i].used) out << table1[i].key << ":" << table1[i].value << " ";
	}
	for (int i = 0; i < TABLE_SIZE; ++i) {
		if (table2[i].used) out << table2[i].key << ":" << table2[i].value << " ";
	}
	out << "\n";
}

void HashMapCuckoo::load(istream& in) {
	clear();
	int n;
	if (!(in >> n)) return;
	for (int i = 0; i < n; ++i) {
		string kv;
		if (in >> kv) {
			size_t pos = kv.find(':');
			string k = kv.substr(0, pos);
			string v = kv.substr(pos + 1);
			put(k, v);
		}
	}
}

static HashMapChaining h1;
static HashMapCuckoo h2;

bool handleHashCommand(const string& cmd, istringstream& iss) {
	string k, v;
	if (cmd == "H1_SET") { iss >> k >> v; h1.put(k, v); return true; }
	if (cmd == "H1_GET") { iss >> k; if (h1.get(k, v)) cout << v << "\n"; else cout << "NOT_FOUND\n"; return true; }
	if (cmd == "H1_DEL") { iss >> k; cout << (h1.remove(k) ? "TRUE" : "FALSE") << "\n"; return true; }
	if (cmd == "H1_CLEAR") { h1.clear(); return true; }
	if (cmd == "H2_SET") { iss >> k >> v; h2.put(k, v); return true; }
	if (cmd == "H2_GET") { iss >> k; if (h2.get(k, v)) cout << v << "\n"; else cout << "NOT_FOUND\n"; return true; }
	if (cmd == "H2_DEL") { iss >> k; cout << (h2.remove(k) ? "TRUE" : "FALSE") << "\n"; return true; }
	if (cmd == "H2_CLEAR") { h2.clear(); return true; }
	return false;
}

void saveHashState(ostream& out) {
	out << "H1 ";
	h1.save(out);
	out << "H2 ";
	h2.save(out);
}

void loadHashState(istream& in) {
	string type;
	if (!(in >> type)) return;
	if (type == "H1") h1.load(in);
	if (!(in >> type)) return;
	if (type == "H2") h2.load(in);
}
