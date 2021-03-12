#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>
#include "DatabaseAccess.h"

using namespace Pistache;


struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)
    void onRequest(const Http::Request&, Http::ResponseWriter writer) override{

        DatabaseAccess db = DatabaseAccess();

        vector<vector<string> > v = db.selectQuery("select * from TestModel;");

        for (auto it: v) {
            for (auto col: it) {
                cout << col << " ";
            }
            cout << endl;
        }

        writer.send(Http::Code::Ok, "Hello, World!");
    }

};

int main() {
    Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));
}