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
const string CLIENT_ID		{ "paho_cpp_async_consume" };
const string TOPIC 			{ "hello" };

const int  QOS = 1;

class SmartFridgeMqttClient {
private:

public:
    SmartFridgeMqttClient() {
    }

    void run() {
        mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
        auto connOpts = mqtt::connect_options_builder()
                .clean_session(false)
                .finalize();

        try {
            // Start consumer before connecting to make sure to not miss messages

            client.start_consuming();

            // Connect to the server

            cout << "Connecting to the MQTT server..." << flush;
            auto tok = client.connect(connOpts);

            // Getting the connect response will block waiting for the
            // connection to complete.
            auto rsp = tok->get_connect_response();

            // If there is no session present, then we need to subscribe, but if
            // there is a session, then the server remembers us and our
            // subscriptions.
            if (!rsp.is_session_present())
                client.subscribe(TOPIC, QOS)->wait();

            cout << "OK" << endl;

            // Consume messages
            // This just exits if the client is disconnected.
            // (See some other examples for auto or manual reconnect)

            cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;

            while (true) {
                auto msg = client.consume_message();
                if (!msg) break;
                cout << msg->get_topic() << ": " << msg->to_string() << endl;
            }

            // If we're here, the client was almost certainly disconnected.
            // But we check, just to make sure.

            if (client.is_connected()) {
                cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
                client.unsubscribe(TOPIC)->wait();
                client.stop_consuming();
                client.disconnect()->wait();
                cout << "OK" << endl;
            }
            else {
                cout << "\nClient was disconnected" << endl;
            }
        }
        catch (const mqtt::exception& exc) {
            cerr << "\n  " << exc << endl;
            return ;
        }
    }
};


#endif //SMART_FRIDGE_API_SMARTFRIDGEMQTTCLIENT_H
