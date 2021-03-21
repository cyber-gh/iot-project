//
// Created by Soltan Gheorghe on 16.03.2021.
//

#ifndef SMART_FRIDGE_API_SMARTFRIDGEMQTTCLIENT_H
#define SMART_FRIDGE_API_SMARTFRIDGEMQTTCLIENT_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"

using namespace std;

const string SERVER_ADDRESS	{ "tcp://localhost:1883" };
const string CLIENT_ID		{ "smart_fridge_client" };
const string TOPIC 			{ "fridge_commands" };
const string STATUS_TOPIC {"status"};
const auto DURATION = std::chrono::seconds(60);


const int  QOS = 1;

class SmartFridgeMqttClient {
private:


public:
    mqtt::async_client *client;
    SmartFridgeMqttClient(string server_address = SERVER_ADDRESS, string client_id = CLIENT_ID){
        client = new mqtt::async_client(server_address, client_id);

    }

    void runListener();
    void runPublisher();
};


#endif //SMART_FRIDGE_API_SMARTFRIDGEMQTTCLIENT_H
