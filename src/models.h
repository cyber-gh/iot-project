//
// Created by Soltan Gheorghe on 12.03.2021.
//

#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;

struct TestModel {
    int id;
    string name;

    TestModel(int id, const string &name) : id(id), name(name) {}

    static TestModel parse(const vector<string> &v) {
        if (v.size() != 2) {
            throw "Invalid TestModel, cannot parse";
        }

        return TestModel(std::stoi(v[0]), v[1]);
    }
};


struct Product {
    int id;
    string name;
    int quantity;
    string date; // YYYY-MM-DD format, expiration date
    int maxTemp;

    Product(int id, const string &name, int quantity, const string &date, int maxTemp) :
            id(id), name(name), quantity(quantity), date(date), maxTemp(maxTemp) {}

    static Product parse(const vector<string> &v) {
        if (v.size() != 5) {
            throw "Invalid Product, cannot parse";
        }

        return Product(stoi(v[0]), v[1], stoi(v[2]), v[3], stoi(v[4]));
    }

    string genInsertQuery() {
        string query = "";
        query += "INSERT INTO Products (id, name, quantity, date, max_temp)\nVALUES(";
        query += to_string(id) + ", ";
        query += name + ", ";
        query += to_string(quantity) + ", ";
        query += date + ", ";
        query += to_string(maxTemp) + ");";

        return query;
    }

    string genDeleteQuery() {
        string query = "";
        query += "DELETE FROM Products where id=";
        query += to_string(id);

        return query;
    }

    string genGetQuery() {
        string query = "";
        query += "SELECT * FROM Products";

        return query;
    }
};


struct Fridge {
    int id;
    int temp;

    Fridge(int id, int temp) :
        id(id), temp(temp) {}

    static Fridge parse(const vector<string> &v) {
        if (v.size() != 2) {
            throw "Invalid fridge, cannot parse";
        }

        return Fridge(stoi(v[0]), stoi(v[1]));
    }

    string genInsertQuery() {
        string query = "";
        query += "INSERT INTO fridges (id, temp)\nVALUES(";
        query += to_string(id) + ", ";
        query += to_string(temp) + ");";
        return query;
    }

    string genGetTempQuery() {
        string query = "";
        query += "SELECT * FROM fridges WHERE id = ";
        query += to_string(id) + ";";
        return query;
    }

    string genSetTempQuery(int newTemp) {
        string query = "";
        query += "UPDATE fridges\nSET temp = ";
        query += to_string(newTemp) + "\nWHERE id = ";
        query += to_string(id) + ";";
        return query;
    }
};

void to_json(nlohmann::json& j, const TestModel& p);
void to_json(nlohmann::json& j, const Fridge& f);
void to_json(nlohmann::json& j, const Product& p);


