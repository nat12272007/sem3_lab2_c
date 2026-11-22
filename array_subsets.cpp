#include "array_subsets.h"

bool handleTask4Command(Array& arr, const string& cmd, istringstream& iss) {
	if (cmd == "T4_ADD") {
		string v; if (!(iss >> v)) return true;
		arr.pushBack(v);
		return true;
	}
	if (cmd == "T4_CLEAR") { arr.clear(); return true; }
	if (cmd == "T4_SUBSETS") {
		int n = arr.length();
		int total = 1 << n;
		cout << "[";
		for (int mask = 0; mask < total; ++mask) {
			if (mask > 0) cout << ", ";
			cout << "{";
			bool first = true;
			for (int i = 0; i < n; ++i) {
				if (mask & (1 << i)) {
					if (!first) cout << ", ";
					cout << arr.get(i);
					first = false;
				}
			}
			cout << "}";
		}
		cout << "]\n";
		return true;
	}
	return false;
}
