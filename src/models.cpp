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

void to_json(nlohmann::json& j, const Product& p) {
    j = {
            {"id", p.id},
            {"name", p.name},
            {"quantity", p.quantity},
            {"date", p.date},
            {"max_temp", p.maxTemp}
    };
}

void to_json(nlohmann::json& j, const Fridge& f) {
    j = {
            {"id", f.id},
            {"temp", f.temp}
    };
}

void from_json(const nlohmann::json& j, Product& p) {
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    j.at("quantity").get_to(p.quantity);
    j.at("date").get_to(p.date);
    j.at("max_temp").get_to(p.maxTemp);
}
