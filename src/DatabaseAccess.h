//
// Created by Soltan Gheorghe on 12.03.2021.
//

#pragma once

#include <iostream>
#include <vector>
#include <sqlite3.h>

using namespace std;

class DatabaseAccess {
public:
    vector<vector<string>> selectQuery(const string query);
    void insertQuery(const string &query);
    void deleteQuery(const string &query);
    void updateQuery(const string &query);
    void createQuery(const string &query);
};


