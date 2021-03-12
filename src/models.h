//
// Created by Soltan Gheorghe on 12.03.2021.
//

#ifndef PISTACHE_CPP_EXAMPLE_MODELS_H
#define PISTACHE_CPP_EXAMPLE_MODELS_H

#include <iostream>
#include <vector>

using namespace std;

struct TestModel {
    int id;
    string name;

    TestModel(int id, const string &name) : id(id), name(name) {}

    TestModel parse(const vector<string> &v) {
        if (v.size() != 2) {
            throw "Invalid TestModel dat, canot parse";
        }

        return TestModel(std::stoi(v[0]), v[1]);
    }
};


#endif //PISTACHE_CPP_EXAMPLE_MODELS_H
