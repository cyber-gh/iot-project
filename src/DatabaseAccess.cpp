//
// Created by Soltan Gheorghe on 12.03.2021.
//

#include "DatabaseAccess.h"

DatabaseAccess* DatabaseAccess::instance = 0;

DatabaseAccess DatabaseAccess::getInstance() {
    if (!instance) instance = new DatabaseAccess();

    return *instance;
}

static int selectCallback(void* data, int argc, char** argv, char** azColName){
    vector<vector<string> > *v = (vector<vector<string> > *) data;
    v->push_back(vector<string>());
    for (int i = 0; i < argc; i++) {
        v->back().push_back(argv[i]);
    }
    return 0;
}


static int executeCallback(void *data, int argc, char **argv, char **azColName) {
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
    int rc = sqlite3_exec(DB, sql.c_str(), selectCallback, (void *)&data, NULL);

    if (rc != SQLITE_OK)  {
        cerr << sqlite3_errmsg(DB);
        cerr << "Error SELECT" << endl;
    }

    sqlite3_close(DB);

    return data;
}

void DatabaseAccess::executeQuery(const string &query) {
    sqlite3 *db;
    int exit = 0;
    const char *sql = query.c_str();

    /* Open database */
    exit = sqlite3_open("database/db.sqlite", &db);

    if (exit) {
        fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Opened database successfully!\n");
    }

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, executeCallback, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        fprintf(stdout, "SQL error\n");
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

    sqlite3_close(db);
}