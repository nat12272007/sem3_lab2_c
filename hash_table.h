#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <string>
#include <sstream>
#include "forward_list.h"

using namespace std;

class HashMapChaining {
private:
	static const int NUM_BUCKETS = 1000;
	ForwardList buckets[NUM_BUCKETS];
	int size;
	int hash(const string& key) const;
public:
	HashMapChaining();
	void put(const string& key, const string& value);
	bool get(const string& key, string& value) const;
	bool remove(const string& key);
	bool contains(const string& key) const;
	int getSize() const { return size; }
	void clear();
	void save(ostream& out) const;
	void load(istream& in);
};

class HashMapCuckoo {
private:
	static const int TABLE_SIZE = 1000;
	static const int MAX_LOOP = 500;
	struct Entry { string key; string value; bool used; };
	Entry table1[TABLE_SIZE];
	Entry table2[TABLE_SIZE];
	int size;
	int hash1(const string& key) const;
	int hash2(const string& key) const;
public:
	HashMapCuckoo();
	void put(const string& key, const string& value);
	bool get(const string& key, string& value) const;
	bool remove(const string& key);
	bool contains(const string& key) const;
	int getSize() const { return size; }
	void clear();
	void save(ostream& out) const;
	void load(istream& in);
};

bool handleHashCommand(const string& cmd, istringstream& iss);
void saveHashState(ostream& out);
void loadHashState(istream& in);

#endif
