//
// Created by Soltan Gheorghe on 12.03.2021.
//

#include "DatabaseAccess.h"

static int callback(void* data, int argc, char** argv, char** azColName){
    vector<vector<string> > *v = (vector<vector<string> > *) data;
    v->push_back(vector<string>());
    for (int i = 0; i < argc; i++) {
        v->back().push_back(argv[i]);
    }
    return 0;
}

vector<vector<string>> DatabaseAccess::selectQuery(const string query) {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("database/db.sqlite", &DB);

    vector<vector<string> > data = vector<vector<string> >();

    string sql = query;
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;

    int rc = sqlite3_exec(DB, sql.c_str(), callback, (void *)&data, NULL);

    if (rc != SQLITE_OK)
        cerr << "Error SELECT" << endl;
    else {
        cout << "Operation OK!" << endl;
    }

    sqlite3_close(DB);

    return data;
}


