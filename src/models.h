//
// Created by Soltan Gheorghe on 12.03.2021.
//

#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "DatabaseAccess.h"

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

struct Search {

    string formatString(const string &stringValue) {
        return "'" + stringValue + "'";
    }

    string genSearchFilter(const string &productName) {
        string query = "";
        query += "SELECT * FROM Products WHERE name=";
        query += formatString(productName);

        return query;
    }

    string genSearchFilter(const int &maxQuantity) {
        string query = "";
        query += "SELECT * FROM Products WHERE quantity <=";
        query += to_string(maxQuantity);

        return query;
    }

    string genSearchFilter() {
        string query = "";
        query += "SELECT * FROM Products";

        return query;
    }

    string genDeleteProductQuery(const string &productName) {
        string query = "";
        query += "DELETE FROM Products where name=";
        query += formatString(productName);

        return query;
    }
};


struct Product {
    int id;
    string name;
    int quantity;
    string date; // YYYY-MM-DD format, expiration date
    int maxTemp;

    string formatString(const string &stringValue) {
        return "'" + stringValue + "'";
    }

    Product() : id(-1), name(""), quantity(-1), date("0000-00-00"), maxTemp(-1) {}

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
        query += "INSERT INTO Products (id, name, quantity, date, max_temp) VALUES (";
        query += to_string(id) + ", ";
        query += formatString(name) + ", ";
        query += to_string(quantity) + ", ";
        query += formatString(date) + ", ";
        query += to_string(maxTemp) + ");";

        return query;
    }

    string genDeleteQuery() {
        string query = "";
        query += "DELETE FROM Products where id=";
        query += to_string(id);

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

    static int getTemperature() {

        DatabaseAccess db = DatabaseAccess::getInstance();
        string query = Fridge::getTempQuery();
        vector<vector<string>> v = db.selectQuery(query);
        Fridge ans = Fridge::parse(v[0]);

        return ans.temp;
    }   

    string genInsertQuery() {
        string query = "";
        query += "INSERT INTO fridges (id, temp)\nVALUES(";
        query += to_string(id) + ", ";
        query += to_string(temp) + ");";
        return query;
    }

    static string getTempQuery() { // id = 1
        return "SELECT * FROM fridges WHERE id = 1;";
    }

    string genGetTempQuery() {
        string query = "";
        query += "SELECT * FROM fridges WHERE id = ";
        query += to_string(id) + ";";
        return query;
    }

    static string setTempQuery(int newTemp) { // id = 1
        string query = "";
        query += "UPDATE fridges\nSET temp = ";
        query += to_string(newTemp) + "\nWHERE id = 1;";
        return query;
    }

    string genSetTempQuery(int newTemp) {
        string query = "";
        query += "UPDATE fridges\nSET temp = ";
        query += to_string(newTemp) + "\nWHERE id = ";
        query += to_string(id) + ";";
        return query;
    }

    static string setEcoTempQuery() { // id = 1
        string query = "";
        query += "UPDATE fridges\nSET temp = ";
        query += "(SELECT MIN(max_temp) FROM Products)\n";
        query += "WHERE id = ";
        query += to_string(1) + ";";
        return query;
    }

    static string chooseProductByMinDate(string p1Name, string p2Name) {
        string subQuery = "(SELECT min(date) FROM Products\n";
        subQuery += "WHERE name = '" + p1Name + "' or name = '" + p2Name + "' );";
        string query = "";
        query += "SELECT name FROM Products \n";
        query += "WHERE date = " + subQuery;
        return query;
    }

    static string selectProductByMinDate() {
        string subQuery = "(SELECT min(date) FROM Products);\n";
        string query = "SELECT name FROM Products \n";
        query += "WHERE date = " + subQuery;
        return query;
    }

    static string selectAllProductsNames() {
        string query = "SELECT name FROM Products;\n";
        return query;
    }

    static int levDist(string s1, string s2) {
        int m = s1.size();
        int n = s2.size();

        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0) {
                    dp[i][j] = j;
                }
                else if (j == 0) {
                    dp[i][j] - i;
                }
                else if(s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else {
                    dp[i][j] = 1 + min(dp[i][j - 1], min(dp[i - 1][j], dp[i - 1][j - 1]));
                }
            }
        }

        return dp[m][n];
    }

    static string getSimilarWord(string s, vector<string> words) {
        string res = words[0];
        int dist = levDist(s, res);
        for (auto word: words) {
            int tmp_dist = levDist(s, word);
            if (tmp_dist < dist) {
                dist = tmp_dist;
                res = word;
            }
        }
        return res;
    }

};

struct InputStructure {
    string status; // INSERT, DELETE, SETFRIDGE, INCREASE
    string value;
};

void to_json(nlohmann::json& j, const TestModel& p);
void to_json(nlohmann::json& j, const Fridge& f);
void to_json(nlohmann::json& j, const Product& p);


void from_json(const nlohmann::json& j, Product& p);
void from_json(const nlohmann::json& j, InputStructure& s);

