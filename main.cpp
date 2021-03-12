#include <pistache/endpoint.h>
#include <sqlite3.h>
#include <nlohmann/json.hpp>
#include <iostream>


using namespace Pistache;
using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}
struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)
    void onRequest(const Http::Request&, Http::ResponseWriter writer) override{
        sqlite3* DB;
        int exit = 0;
        exit = sqlite3_open("db.sqlite", &DB);
        string data("CALLBACK FUNCTION");

        string sql("SELECT * FROM TestModel;");
        if (exit) {
            std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
//            return (-1);
        }
        else
            std::cout << "Opened Database Successfully!" << std::endl;

        int rc = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);

        if (rc != SQLITE_OK)
            cerr << "Error SELECT" << endl;
        else {
            cout << "Operation OK!" << endl;
        }

        sqlite3_close(DB);
        writer.send(Http::Code::Ok, "Hello, World!");
    }
};

int main() {
    Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));
}