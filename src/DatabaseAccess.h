//
// Created by Soltan Gheorghe on 12.03.2021.
//

#ifndef PISTACHE_CPP_EXAMPLE_DATABASEACCESS_H
#define PISTACHE_CPP_EXAMPLE_DATABASEACCESS_H

#include <iostream>
#include <vector>
#include <sqlite3.h>

using namespace std;

class DatabaseAccess {
public:
    vector<vector<string>> selectQuery(const string query);
};


#endif //PISTACHE_CPP_EXAMPLE_DATABASEACCESS_H
