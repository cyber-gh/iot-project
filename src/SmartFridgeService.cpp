//
// Created by Soltan Gheorghe on 15.03.2021.
//

#include "SmartFridgeService.h"

using json = nlohmann::json;

void SmartFridgeService::setupRoutes() {
    using namespace Rest;

    Routes::Get(router, "/products", Routes::bind(&SmartFridgeService::getAllProducts, this));
    Routes::Post(router, "/setTemperature/:temp", Routes::bind(&SmartFridgeService::setTemperature, this));
    Routes::Get(router, "/getTemperature", Routes::bind(&SmartFridgeService::getTemperature, this));
    Routes::Post(router, "/products", Routes::bind(&SmartFridgeService::insertProduct, this));
}

void addJsonContentTypeHeader(Http::ResponseWriter &response) {
    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}


void SmartFridgeService::getAllProducts(const Rest::Request &request, Http::ResponseWriter response) {
    DatabaseAccess db = DatabaseAccess::getInstance();


    vector<vector<string>> v = db.selectQuery("select * from Products;");
    vector<Product> ans;

    for (auto it: v) {
        ans.push_back(Product::parse(it));
    }

    json j = ans;
    addJsonContentTypeHeader(response);
    response.send(Http::Code::Ok, j.dump());
}


void SmartFridgeService::setTemperature(const Rest::Request &request, Http::ResponseWriter response) { 
    //curl -X POST localhost:9080/setTemperature/20
    DatabaseAccess db = DatabaseAccess::getInstance();
    auto temp = request.param(":temp").as<int>();
    string query = Fridge::setTempQuery(temp);
    db.executeQuery(query);
    addJsonContentTypeHeader(response);
    response.send(Http::Code::Ok);
}



void SmartFridgeService::getTemperature(const Rest::Request &request, Http::ResponseWriter response) {
    //curl localhost:9080/getTemperature
    DatabaseAccess db = DatabaseAccess::getInstance();
    string query = Fridge::getTempQuery();
    vector<vector<string>> v = db.selectQuery(query);
    Fridge ans = Fridge::parse(v[0]);

    json j = ans.temp;
    addJsonContentTypeHeader(response);
    response.send(Http::Code::Ok, j.dump());
}

void SmartFridgeService::insertProduct(const Rest::Request &request, Http::ResponseWriter response) {
    cout << request.body() << '\n';
    response.send(Http::Code::Ok);
}