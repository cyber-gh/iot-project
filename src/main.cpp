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

        vector<vector<string> > v = db.selectQuery("select * from TestModel;");

        vector<TestModel> ans;
        for (auto it: v) {
            ans.push_back(TestModel::parse(it));
        }

        json j = ans;
        writer.send(Http::Code::Ok, j.dump());
    }

};

int main() {
    Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));
}