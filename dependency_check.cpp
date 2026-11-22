#include "dependency_check.h"
#include "forward_list.h"
#include "stack.h"

static ForwardList adj[26];
static bool existsTask[26];
static int state[26];
static bool cycle = false;

static void dfs(int u) {
	if (state[u] == 1) { cycle = true; return; }
	if (state[u] == 2) return;
	state[u] = 1;
	FNode* cur = adj[u].getHead();
	while (cur) {
		char v = cur->value[0];
		dfs(v - 'A');
		if (cycle) return;
		cur = cur->next;
	}
	state[u] = 2;
}

static void resetTask1() {
	for (int i = 0; i < 26; ++i) {
		adj[i].clear();
		existsTask[i] = false;
		state[i] = 0;
	}
	cycle = false;
}

bool handleTask1Command(const string& cmd, istringstream& iss) {
	if (cmd == "T1_RESET") { resetTask1(); return true; }
	if (cmd == "T1_ADDTASK") {
		char c; if (!(iss >> c)) return true;
		int idx = c - 'A';
		if (idx >= 0 && idx < 26) existsTask[idx] = true;
		return true;
	}
	if (cmd == "T1_DEP") {
		char a, b; if (!(iss >> a >> b)) return true;
		int ai = a - 'A', bi = b - 'A';
		if (!existsTask[ai]) existsTask[ai] = true;
		if (!existsTask[bi]) existsTask[bi] = true;
		string val(1, a);
		adj[bi].pushBack(val);
		return true;
	}
	if (cmd == "T1_CHECK") {
		for (int i = 0; i < 26; ++i) state[i] = 0;
		cycle = false;
		for (int i = 0; i < 26; ++i) {
			if (existsTask[i] && state[i] == 0) dfs(i);
			if (cycle) break;
		}
		cout << (cycle ? "Невозможно" : "Возможно") << "\n";
		return true;
	}
	return false;
}

void saveDependencyState(ostream& out) {
	int n = 0;
	for (int i = 0; i < 26; ++i) if (existsTask[i]) n++;
	out << "Задача1 " << n << "\n";
	for (int i = 0; i < 26; ++i) if (existsTask[i]) out << (char)('A' + i) << " ";
	out << "\n";
	int e = 0;
	for (int i = 0; i < 26; ++i) e += adj[i].getCount();
	out << "Зависимости " << e << "\n";
	for (int i = 0; i < 26; ++i) {
		for (FNode* cur = adj[i].getHead(); cur; cur = cur->next) {
			out << (char)('A' + i) << " " << cur->value << " ";
		}
	}
	out << "\n";
}

void loadDependencyState(istream& in) {
	string type; int n;
	if (!(in >> type >> n)) return;
	if (type == "Задача1") {
		for (int i = 0; i < n; ++i) {
			char c; in >> c;
			existsTask[c - 'A'] = true;
		}
	}
	if (!(in >> type >> n)) return;
	if (type == "Зависимости") {
		for (int i = 0; i < n; ++i) {
			char b, a; in >> b >> a;
			adj[b - 'A'].pushBack(string(1, a));
		}
	}
}
