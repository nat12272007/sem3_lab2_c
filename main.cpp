#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "array.h"
#include "forward_list.h"
#include "doubly_list.h"
#include "stack.h"
#include "queue.h"
#include "complete_binary_tree.h"
#include "set.h"
#include "dependency_check.h"
#include "set_partition.h"
#include "array_subsets.h"
#include "hash_table.h"
#include "longest_substring.h"
#include "lru_cache.h"

using namespace std;

Array arr;
ForwardList flist;
DoublyList dlist;
Stack stk;
Queue q;
CompleteBinaryTree tree;
Set mySet;

void saveToFile(const string& path) {
	ofstream out(path);
	if (!out) return;

	out << "Массив " << arr.getSize() << "\n";
	for (int i = 0; i < arr.getSize(); ++i) out << arr.getElement(i) << " ";

	out << "\nОднСписок " << flist.getCount() << "\n";
	for (FNode* cur = flist.getHead(); cur; cur = cur->next) out << cur->value << " ";

	out << "\nДвСписок " << dlist.getCount() << "\n";
	for (LNode* cur = dlist.getHead(); cur; cur = cur->next) out << cur->value << " ";

	out << "\nСтек " << (stk.getTop() + 1) << "\n";
	for (int i = 0; i <= stk.getTop(); ++i) out << stk.getElement(i) << " ";

	out << "\nОчередь " << q.getCount() << "\n";
	for (int i = 0; i < q.getCount(); ++i) out << q.getElement(i) << " ";

	out << "\nДерево " << tree.getSize() << "\n";
	vector<int> te = tree.getElements();
	for (int v : te) out << v << " ";

	out << "\nМножество " << mySet.size() << "\n";
	vector<string> se = mySet.getAll();
	for (const string& s : se) out << s << " ";
	out << "\n";

	saveDependencyState(out);
	saveHashState(out);
	saveLRUState(out);
}

void loadFromFile(const string& path) {
	ifstream in(path);
	if (!in) {
		cout << "[Файл не найден, создадим новый: " << path << "]\n";
		return;
	}
	string type;
	int n = 0;
	while (in >> type >> n) {
		if (n < 0 || n > 1000000) n = 0;
		
		if (type == "Массив") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) arr.pushBack(v); }
		}
		else if (type == "ОднСписок") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) flist.pushBack(v); }
		}
		else if (type == "ДвСписок") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) dlist.pushBack(v); }
		}
		else if (type == "Стек") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) stk.push(v); }
		}
		else if (type == "Очередь") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) q.push(v); }
		}
		else if (type == "Дерево") {
			for (int i = 0; i < n; ++i) { int v; if (in >> v) tree.insert(v); }
		}
		else if (type == "Множество") {
			for (int i = 0; i < n; ++i) { string v; if (in >> v) mySet.add(v); }
		}
		else if (type == "Задача1") {
			in.seekg(-(long)(type.length() + to_string(n).length() + 2), ios_base::cur);
			loadDependencyState(in);
		}
		else if (type == "H1") {
			in.seekg(-(long)(type.length() + to_string(n).length() + 2), ios_base::cur);
			loadHashState(in);
		}
		else if (type == "LRU") {
			in.seekg(-(long)(type.length() + to_string(n).length() + 2), ios_base::cur);
			loadLRUState(in);
		}
		else {
			for (int i = 0; i < n; ++i) { string v; if (!(in >> v)) break; }
		}
	}
}

void processCommand(const string& query) {
	istringstream iss(query);
	string cmd;
	iss >> cmd;

	if (cmd == "PRINT") {
		string target; iss >> target;
		if (target == "myarr") arr.print();
		else if (target == "mylist") flist.print();
		else if (target == "mydlist") dlist.print();
		else if (target == "mystack") stk.print();
		else if (target == "myqueue") q.print();
		else if (target == "mytree") tree.print();
		else if (target == "myset") {
			vector<string> e = mySet.getAll();
			cout << "{ "; 
			for (const string& s : e) cout << s << " "; 
			cout << "}\n";
		}
		return;
	}

	if (handleArrayCommand(arr, cmd, iss)) return;
	if (handleForwardListCommand(flist, cmd, iss)) return;
	if (handleDoublyListCommand(dlist, cmd, iss)) return;
	if (handleStackCommand(stk, cmd, iss)) return;
	if (handleQueueCommand(q, cmd, iss)) return;
	if (handleTreeCommand(tree, cmd, iss)) return;
	if (handleSetCommand(mySet, cmd, iss)) return;
	
	if (handleTask1Command(cmd, iss)) return;
	if (handleTask3Command(mySet, cmd, iss)) return;
	if (handleTask4Command(arr, cmd, iss)) return;
	if (handleHashCommand(cmd, iss)) return;
	if (handleTask6Command(cmd, iss)) return;
	if (handleLRUCommand(cmd, iss)) return;

	cout << "[Ошибка: неизвестная команда: " << cmd << "]\n";
}

int main(int argc, char* argv[]) {
	string filePath;
	vector<string> queries;
	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if (arg == "--file" && i + 1 < argc) filePath = argv[++i];
		else if (arg == "--query" && i + 1 < argc) queries.push_back(argv[++i]);
	}
	
	if (filePath.empty()) {
		filePath = "data.txt";
		cout << "[файл не указан, запись сюда: data.txt]\n";
	}
	
	loadFromFile(filePath);
	
	for (auto& qr : queries) {
		processCommand(qr);
	}
	
	saveToFile(filePath);
	cout << "\n[Программа завершена.]\n";
	return 0;
}
