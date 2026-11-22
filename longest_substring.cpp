#include "longest_substring.h"
#include "hash_table.h"

bool handleTask6Command(const string& cmd, istringstream& iss) {
	if (cmd == "LONGEST") {
		string s; if (!(iss >> s)) return true;
		HashMapChaining map;
		int left = 0, maxLen = 0, start = 0;
		for (int right = 0; right < (int)s.size(); ++right) {
			string ch(1, s[right]);
			string lastPosStr;
			if (map.get(ch, lastPosStr)) {
				int lastPos = stoi(lastPosStr);
				if (lastPos >= left) left = lastPos + 1;
			}
			map.put(ch, to_string(right));
			if (right - left + 1 > maxLen) { maxLen = right - left + 1; start = left; }
		}
		cout << "Длина: " << maxLen << "\n";
		if (maxLen > 0) cout << "Подстрока: " << s.substr(start, maxLen) << "\n";
		return true;
	}
	return false;
}
