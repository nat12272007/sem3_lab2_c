#include "array.h"

Array::Array(int cap) {
	capacity = cap;
	size = 0;
	data = new string[capacity];
}

Array::~Array() { delete[] data; }

void Array::ensureCapacity(int needed) {
	if (needed <= capacity) return;
	while (capacity < needed) capacity *= 2;
	string* newData = new string[capacity];
	for (int i = 0; i < size; ++i) newData[i] = data[i];
	delete[] data;
	data = newData;
}

void Array::pushBack(const string& value) {
	ensureCapacity(size + 1);
	data[size++] = value;
}

void Array::pushAt(int index, const string& value) {
	if (index < 0 || index > size) return;
	ensureCapacity(size + 1);
	for (int i = size; i > index; --i) data[i] = data[i - 1];
	data[index] = value;
	++size;
}

string Array::get(int index) const {
	return (index >= 0 && index < size) ? data[index] : "ERROR";
}

void Array::removeAt(int index) {
	if (index < 0 || index >= size) return;
	for (int i = index; i < size - 1; ++i) data[i] = data[i + 1];
	--size;
}

void Array::set(int index, const string& value) {
	if (index >= 0 && index < size) data[index] = value;
}

int Array::length() const { return size; }

void Array::print() const {
	cout << "[";
	for (int i = 0; i < size; ++i) {
		if (i) cout << ", ";
		cout << data[i];
	}
	cout << "]\n";
}

void Array::clear() { size = 0; }

bool handleArrayCommand(Array& arr, const string& cmd, istringstream& iss) {
	string name, v; int i;
	if (cmd == "MPUSH") { iss >> name >> v; arr.pushBack(v); return true; }
	if (cmd == "MINSERT") { iss >> name >> i >> v; arr.pushAt(i, v); return true; }
	if (cmd == "MGET") { iss >> name >> i; cout << arr.get(i) << "\n"; return true; }
	if (cmd == "MSET") { iss >> name >> i >> v; arr.set(i, v); return true; }
	if (cmd == "MDEL") { iss >> name >> i; arr.removeAt(i); return true; }
	if (cmd == "MLEN") { iss >> name; cout << arr.length() << "\n"; return true; }
	return false;
}
