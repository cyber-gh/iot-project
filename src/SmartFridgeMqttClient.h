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
using namespace std::chrono;
const string SERVER_PORT {"1882"};
const string SERVER_ADDRESS	{ "tcp://localhost:" + SERVER_PORT };
const string CLIENT_ID		{ "smart_fridge_client" };
const string PUBLISHER_ID		{ "smart_fridge_client" };
const string TOPIC 			{ "fridge_commands" };



// How often to sample the "data"
const auto SAMPLE_PERIOD = std::chrono::milliseconds(10000);
// How much the "data" needs to change before we publish a new value.
const int DELTA_MS = 10000;
// How many to buffer while off-line
const int MAX_BUFFERED_MESSAGES = 1200;

const string STATUS_TOPIC {"status"};


const int  QOS = 1;

class SmartFridgeMqttClient {
private:
    void process_message(string message);

public:
    mqtt::async_client *client;
    SmartFridgeMqttClient(string server_address = SERVER_ADDRESS, string client_id = CLIENT_ID){
        client = new mqtt::async_client(server_address, client_id);

    }

    void runListener();
    void runPublisher();
};


#endif //SMART_FRIDGE_API_SMARTFRIDGEMQTTCLIENT_H
