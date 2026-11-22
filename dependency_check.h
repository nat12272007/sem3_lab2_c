#ifndef DEPENDENCY_CHECK_H
#define DEPENDENCY_CHECK_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

bool handleTask1Command(const string& cmd, istringstream& iss);
void saveDependencyState(ostream& out);
void loadDependencyState(istream& in);

#endif
