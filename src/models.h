//
// Created by Soltan Gheorghe on 12.03.2021.
//

#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

struct TestModel {
    int id;
    std::string name;

    TestModel(int id, const std::string &name) : id(id), name(name) {}

    static TestModel parse(const std::vector<std::string> &v) {
        if (v.size() != 2) {
            throw "Invalid TestModel, cannot parse";
        }

        return TestModel(std::stoi(v[0]), v[1]);
    }
};

struct Product {
    int id;
    std::string name;
    int quantity;
    std::string date; // YYYY-MM-DD format, expiration date
    int maxTemp;

    Product(int id, const std::string &name, int quantity, const std::string &date, int maxTemp) :
        id(id), name(name), quantity(quantity), date(date), maxTemp(maxTemp) {}

    static Product parse(const std::vector<std::string> &v) {
        if (v.size() != 5) {
            throw "Invalid Product, cannot parse";
        }

        return Product(std::stoi(v[0]), v[1], std::stoi(v[2]), v[3], std::stoi(v[4]));
    }

    std::string genInsertQuery() {
        std::string query = "";
        query += "INSERT INTO Products (id, name, quantity, date, max_temp)\nVALUES(";
        query += std::to_string(id) + ", ";
        query += name + ", ";
        query += std::to_string(quantity) + ", ";
        query += date + ", ";
        query += std::to_string(maxTemp) + ");";

        return query;
    }
};

void to_json(nlohmann::json& j, const TestModel& p);
void to_json(nlohmann::json& j, const Product& p);


