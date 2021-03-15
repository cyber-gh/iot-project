<<<<<<< HEAD
#include <pistache/endpoint.h>
#include "DatabaseAccess.h"
#include "models.h"
#include "vector"
#include "SmartFridgeService.h"

using namespace Pistache;
using namespace std;

using json = nlohmann::json;

struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)
    void onRequest(const Http::Request&, Http::ResponseWriter writer) override{

        DatabaseAccess db = DatabaseAccess();
        Product product1 = Product(100, "Mihai1", 3, "2021-03-20", 30);
        Product product2 = Product(200, "Mihai2", 12, "2021-04-17", 30);

        string query = product1.genInsertQuery();
        db.insertQuery(query);


        query = product2.genInsertQuery();
        db.insertQuery(query);

        query = product1.genDeleteQuery();
        db.deleteQuery(query);

        Search searcher = Search();

        query = searcher.genSearchFilter("Pere");
        vector<vector<string> > v = db.selectQuery(query);

        vector<Product> ans;
        for (auto it: v) {
            ans.push_back(Product::parse(it));
        }

        json j = ans;
        writer.send(Http::Code::Ok, j.dump());
    }


};

int main() {

    SmartFridgeService service = SmartFridgeService();
    service.start();
}