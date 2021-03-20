//
// Created by Soltan Gheorghe on 15.03.2021.
//

#include "SmartFridgeService.h"

using json = nlohmann::json;

void SmartFridgeService::setupRoutes() {
    using namespace Rest;

    Routes::Get(router, "/products", Routes::bind(&SmartFridgeService::getAllProducts, this));
    Routes::Post(router, "/products", Routes::bind(&SmartFridgeService::insertProduct, this));
    Routes::Delete(router, "/products/:pName?", Routes::bind(&SmartFridgeService::deleteProduct, this));

    Routes::Get(router, "/products/:pName", Routes::bind(&SmartFridgeService::getAProduct, this));

    Routes::Post(router, "/setTemperature/:temp", Routes::bind(&SmartFridgeService::setTemperature, this));
    Routes::Get(router, "/getTemperature", Routes::bind(&SmartFridgeService::getTemperature, this));

    Routes::Put(router, "/fridge/eco", Routes::bind(&SmartFridgeService::setEcoMode, this));
    Routes::Get(router, "/fridge/recommendProduct/:p1Name?/:p2Name?", Routes::bind(&SmartFridgeService::recommendProduct, this));
}

void addJsonContentTypeHeader(Http::ResponseWriter &response) {
    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}


void SmartFridgeService::getAllProducts(const Rest::Request &request, Http::ResponseWriter response) {
    addJsonContentTypeHeader(response);

    DatabaseAccess db = DatabaseAccess::getInstance();


    vector<vector<string>> v = db.selectQuery("select * from Products;");
    vector<Product> ans;

    for (auto it: v) {
        ans.push_back(Product::parse(it));
    }

    json j = ans;
    response.send(Http::Code::Ok, j.dump());
}

void SmartFridgeService::getAProduct(const Rest::Request &request, Http::ResponseWriter response) {
    addJsonContentTypeHeader(response);

    if (!request.hasParam(":pName")) {    
        response.send(Http::Code::Bad_Request);
        return;
    }
    Search searcher = Search();
    string pName = request.param(":pName").as<string>();

    DatabaseAccess db = DatabaseAccess::getInstance();

    auto query = searcher.genSearchFilter(pName);


    vector<vector<string>> v = db.selectQuery(query);
    vector<Product> ans;

    for (auto it: v) {
        ans.push_back(Product::parse(it));
    }

    json j = ans;
    response.send(Http::Code::Ok, j.dump());
}


void SmartFridgeService::setTemperature(const Rest::Request &request, Http::ResponseWriter response) { 
    //curl -X POST localhost:9080/setTemperature/20
    addJsonContentTypeHeader(response);

    DatabaseAccess db = DatabaseAccess::getInstance();
    auto temp = request.param(":temp").as<int>();
    string query = Fridge::setTempQuery(temp);
    db.executeQuery(query);
    response.send(Http::Code::Ok);
}



void SmartFridgeService::getTemperature(const Rest::Request &request, Http::ResponseWriter response) {
    //curl localhost:9080/getTemperature
    addJsonContentTypeHeader(response);

    DatabaseAccess db = DatabaseAccess::getInstance();
    string query = Fridge::getTempQuery();
    vector<vector<string>> v = db.selectQuery(query);
    Fridge ans = Fridge::parse(v[0]);

    json j = ans.temp;
    response.send(Http::Code::Ok, j.dump());
}

void SmartFridgeService::deleteProduct(const Rest::Request &request, Http::ResponseWriter response) {
    addJsonContentTypeHeader(response);

    try {
        if (!request.hasParam(":pName")) {    
            response.send(Http::Code::Bad_Request);
            return;
        }
        Search searcher = Search();
        string pName = request.param(":pName").as<string>();
        auto query = searcher.genDeleteProductQuery(pName);
        cout << query;
        DatabaseAccess db = DatabaseAccess::getInstance();
        db.executeQuery(query);
    } catch (...) {
        response.send(Http::Code::Bad_Request);
        return;
    }

    response.send(Http::Code::Ok);
}

void SmartFridgeService::insertProduct(const Rest::Request &request, Http::ResponseWriter response) {
    // curl -X POST -H "Content-Type: application/json" -d '{"date":"2021-03-23","id":1,"max_temp":20,"name":"test","quantity":3}' localhost:9080/products
    addJsonContentTypeHeader(response);

    try {
        auto bodyJson = json::parse(request.body());
        auto product = bodyJson.get<Product>();

        DatabaseAccess db = DatabaseAccess::getInstance();
        auto query = product.genInsertQuery();

        db.executeQuery(query);
    } catch (...) {
        response.send(Http::Code::Bad_Request);
        return;
    }

    response.send(Http::Code::Ok);
}

void SmartFridgeService::setEcoMode(const Rest::Request &request, Http::ResponseWriter response) {
//    curl -X PUT localhost:9080/fridge/eco
    addJsonContentTypeHeader(response);

    try {
        DatabaseAccess db = DatabaseAccess::getInstance();
        string query = Fridge::setEcoTempQuery();
        db.executeQuery(query);

    } catch (...) {
        response.send(Http::Code::Internal_Server_Error);
        return;
    }

    response.send(Http::Code::Ok);
}

void SmartFridgeService::recommendProduct(const Rest::Request &request, Http::ResponseWriter response) {
    addJsonContentTypeHeader(response);

    try {
        DatabaseAccess db = DatabaseAccess::getInstance();
        if (request.hasParam(":p1Name") && request.hasParam(":p2Name")) {
            string p1Name = request.param(":p1Name").as<string>();
            string p2Name = request.param(":p2Name").as<string>();

            string query = Fridge::chooseProductByMinDate(p1Name, p2Name);
            vector<vector<string>> v = db.selectQuery(query);
            if (v.size() > 0) {
                auto ans = v[0];
                json j = ans;
                response.send(Http::Code::Ok, j.dump());
            } else {
                vector<string> ans;
                query = Fridge::selectAllProductsNames();
                v = db.selectQuery(query);
                if (v.size() == 0) {
                    ans.push_back("Fridge is empty");
                    json j = ans;
                    response.send(Http::Code::Ok, j.dump());
                } else {
                    string simP1 = Fridge::getSimilarWord(p1Name, v[0]);
                    string simP2 = Fridge::getSimilarWord(p2Name, v[0]);
                    query = Fridge::chooseProductByMinDate(simP1, simP2);
                    v = db.selectQuery(query);
                    ans = v[0];
                }
                json j = ans;
                response.send(Http::Code::Ok, j.dump());
            }
            return;
        } else if (!request.hasParam(":p1Name") && !request.hasParam(":p2Name")){
            string query = Fridge::selectProductByMinDate();
            vector<vector<string>> v = db.selectQuery(query);
            auto ans = v[0];

            json j = ans;
            response.send(Http::Code::Ok, j.dump());
            return;
        }
    } catch (...) {
        response.send(Http::Code::Internal_Server_Error);
        return;
    }

    response.send(Http::Code::Bad_Request);
}