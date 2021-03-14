#include <pistache/endpoint.h>
#include "DatabaseAccess.h"
#include "models.h"
#include "vector"

using namespace Pistache;
using namespace std;

using json = nlohmann::json;

struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)
    void onRequest(const Http::Request&, Http::ResponseWriter writer) override{

        DatabaseAccess db = DatabaseAccess();
        Fridge fridge = Fridge(1, 232);
        static int t = 13;

        string query = fridge.genSetTempQuery(t);
        db.updateQuery(query);

        query = fridge.setEcoTempQuery();
        db.updateQuery(query);

        query = fridge.genGetTempQuery();
        vector<vector<string> > v = db.selectQuery(query);
        t = t - 1;

        vector<Fridge> ans;
        for (auto it: v) {
            ans.push_back(Fridge::parse(it));
        }
//        select between two products
//        query = fridge.selectProductByMinDate("Mere", "Pulpe");
//        vector<vector<string>> v = db.selectQuery(query);
//        vector<string> ans;
//
//        for (auto it: v) {
//            ans.push_back(it[0]);
//        }

        json j = ans;
        writer.send(Http::Code::Ok, j.dump());
    }

};

int main() {
    Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));
}