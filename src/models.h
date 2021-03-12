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
            throw "Invalid TestModel dat, canot parse";
        }

        return TestModel(std::stoi(v[0]), v[1]);
    }
};

void to_json(nlohmann::json& j, const TestModel& p);


