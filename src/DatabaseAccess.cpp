//
// Created by Soltan Gheorghe on 12.03.2021.
//

#include "DatabaseAccess.h"

static int selectCallback(void* data, int argc, char** argv, char** azColName){
    vector<vector<string> > *v = (vector<vector<string> > *) data;
    v->push_back(vector<string>());
    for (int i = 0; i < argc; i++) {
        v->back().push_back(argv[i]);
    }
    return 0;
}

static int insertCallback(void *data, int argc, char **argv, char **azColName) {
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

    int rc = sqlite3_exec(DB, sql.c_str(), selectCallback, (void *)&data, NULL);

    if (rc != SQLITE_OK)
        cerr << "Error SELECT" << endl;
    else {
        cout << "Operation OK!" << endl;
    }

    sqlite3_close(DB);

    return data;
}

void DatabaseAccess::insertQuery(const string &query) {
    sqlite3 *db;
    int exit = 0;
    char *sql;

    /* Open database */
    exit = sqlite3_open("test.db", &db);

    if (exit) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Opened database successfully!\n");
    }

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, insertCallback, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error\n");
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

    sqlite3_close(db);
}


