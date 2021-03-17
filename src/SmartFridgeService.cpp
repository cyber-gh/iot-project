//
// Created by Soltan Gheorghe on 15.03.2021.
//

#include "SmartFridgeService.h"

using json = nlohmann::json;

void SmartFridgeService::setupRoutes() {
    using namespace Rest;

    Routes::Get(router, "/products", Routes::bind(&SmartFridgeService::getAllProducts, this));
    Routes::Post(router, "/products", Routes::bind(&SmartFridgeService::insertProduct, this));

    Routes::Post(router, "/setTemperature/:temp", Routes::bind(&SmartFridgeService::setTemperature, this));
    Routes::Get(router, "/getTemperature", Routes::bind(&SmartFridgeService::getTemperature, this));
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