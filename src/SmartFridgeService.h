//
// Created by Soltan Gheorghe on 15.03.2021.
//

#pragma once
#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>


#include "DatabaseAccess.h"
#include "models.h"
#include "vector"

using namespace Pistache;

class SmartFridgeService {
private:
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    void setupRoutes();

    void getAllProducts(const Rest::Request &request, Http::ResponseWriter response);
    void getProductsFilter(const Rest::Request &request, Http::ResponseWriter response);
    void insertProduct(const Rest::Request &request, Http::ResponseWriter response);
    void deleteProduct(const Rest::Request &request, Http::ResponseWriter response);
    void setTemperature(const Rest::Request &request, Http::ResponseWriter response);
    void getTemperature(const Rest::Request &request, Http::ResponseWriter response);

    void setEcoMode(const Rest::Request &request, Http::ResponseWriter response);
    void recommendProduct(const Rest::Request &request, Http::ResponseWriter response);

    void Test(const Rest::Request &request, Http::ResponseWriter response);

public:
    explicit SmartFridgeService(Address addr = Address(Ipv4::any(), Port(9080))): httpEndpoint(std::make_shared<Http::Endpoint>(addr)) {
        auto opts = Http::Endpoint::options();
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

};
