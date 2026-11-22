#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

bool handleLRUCommand(const string& cmd, istringstream& iss);
void saveLRUState(ostream& out);
void loadLRUState(istream& in);

#endif
