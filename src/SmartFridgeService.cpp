//
// Created by Soltan Gheorghe on 15.03.2021.
//

#include "SmartFridgeService.h"

using json = nlohmann::json;

void SmartFridgeService::setupRoutes() {
    using namespace Rest;

    Routes::Get(router, "/products", Routes::bind(&SmartFridgeService::getAllProducts, this));
}

void SmartFridgeService::getAllProducts(const Rest::Request &request, Http::ResponseWriter response) {
        DatabaseAccess db = DatabaseAccess::getInstance();


        vector<vector<string>> v = db.selectQuery("select * from Products;");
        vector<Product> ans;

        for (auto it: v) {
            ans.push_back(Product::parse(it));
        }

        json j = ans;
        response.send(Http::Code::Ok, j.dump());
}