//
// Created by Soltan Gheorghe on 12.03.2021.
//
#include "models.h"



void to_json(nlohmann::json& j, const TestModel& p) {
    j = {
            {"id", p.id},
            {"name", p.name}
    };
}


void to_json(nlohmann::json& j, const Fridge& f) {
    j = {
            {"id", f.id},
            {"temp", f.temp}
    };
}