#include "set_partition.h"
#include <vector>
#include <algorithm>

static bool partitionRec(vector<int>& nums, vector<vector<int>>& subsets,
                         vector<int>& sums, int target, int index) {
	if (index == (int)nums.size()) {
		for (int s : sums) if (s != target) return false;
		return true;
	}
	int num = nums[index];
	for (int i = 0; i < (int)subsets.size(); ++i) {
		if (sums[i] + num <= target) {
			sums[i] += num;
			subsets[i].push_back(num);
			if (partitionRec(nums, subsets, sums, target, index + 1)) return true;
			sums[i] -= num;
			subsets[i].pop_back();
		}
		if (subsets[i].empty()) break;
	}
	return false;
}

static bool tryPartition(vector<int> nums, int target) {
	int total = 0;
	for (int x : nums) total += x;
	if (target <= 0 || total % target != 0) return false;
	int k = total / target;
	if (k < 2) return false;
	vector<vector<int>> subsets(k);
	vector<int> sums(k, 0);
	sort(nums.rbegin(), nums.rend());
	if (partitionRec(nums, subsets, sums, target, 0)) {
		cout << "Найдено " << k << " подмножеств с суммой " << target << ":\n";
		for (int i = 0; i < k; ++i) {
			cout << "{ ";
			for (int x : subsets[i]) cout << x << " ";
			cout << "}\n";
		}
		return true;
	}
	return false;
}

bool handleTask3Command(Set& s, const string& cmd, istringstream& iss) {
	if (cmd == "T3_ADD") {
		string v; if (!(iss >> v)) return true;
		s.add(v);
		return true;
	}
	if (cmd == "T3_CLEAR") { s.clear(); return true; }
	if (cmd == "T3_PRINT") {
		vector<string> elems = s.getAll();
		cout << "{ ";
		for (const string& e : elems) cout << e << " ";
		cout << "}\n";
		return true;
	}
	if (cmd == "T3_PARTITION") {
		int target; if (!(iss >> target)) return true;
		vector<string> elems = s.getAll();
		vector<int> nums;
		for (const string& str : elems) nums.push_back(stoi(str));
		if (!tryPartition(nums, target))
			cout << "Невозможно разбить на подмножества с суммой " << target << "\n";
		return true;
	}
	return false;
}
