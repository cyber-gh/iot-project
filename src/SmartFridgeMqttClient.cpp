//
// Created by Soltan Gheorghe on 16.03.2021.
//

#include "SmartFridgeMqttClient.h"
#include "models.h"

void process_message(string message) {
	InputStructure is;
	from_json(message, is);
    DatabaseAccess db = DatabaseAccess::getInstance();

	if (is.status == "DELETE") {
        Search searcher = Search();
        auto query = searcher.genDeleteProductQuery(is.value);
        db.executeQuery(query);
	} else if (is.status == "SETFRIDGE") {
		int temp = stoi(is.value);
	    string query = Fridge::setTempQuery(temp);
	    db.executeQuery(query);
	}

}

void SmartFridgeMqttClient::runListener() {
    auto connOpts = mqtt::connect_options_builder()
            .clean_session(false)
            .finalize();

    try {
        // Start consumer before connecting to make sure to not miss messages = milli

        client->start_consuming();

        // Connect to the server

        cout << "Connecting to the MQTT server..." << flush;
        auto tok = client->connect(connOpts);

        // Getting the connect response will block waiting for the
        // connection to complete.
        auto rsp = tok->get_connect_response();

        // If there is no session present, then we need to subscribe, but if
        // there is a session, then the server remembers us and our
        // subscriptions.
        if (!rsp.is_session_present())
            client->subscribe(TOPIC, QOS)->wait();

        cout << "OK" << endl;

        // Consume messages
        // This just exits if the client is disconnected.
        // (See some other examples for auto or manual reconnect)

        cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;

        while (true) {
            auto msg = client->consume_message();
            if (!msg) break;
            //TODO here were parse the messages, from msg->to_string()
            process_message(msg->to_string());
            cout << msg->get_topic() << ": " << msg->to_string() << endl;
        }

        // If we're here, the client was almost certainly disconnected.
        // But we check, just to make sure.

        if (client->is_connected()) {
            cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
            client->unsubscribe(TOPIC)->wait();
            client->stop_consuming();
            client->disconnect()->wait();
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

void SmartFridgeMqttClient::runPublisher() {
    auto connOpts = mqtt::connect_options_builder()
            .clean_session(false)
            .finalize();

    try {

        cout << "Connecting to the MQTT server..." << flush;
        auto tok = client->connect(connOpts);

        // Getting the connect response will block waiting for the
        // connection to complete.
        auto rsp = tok->get_connect_response();

        // If there is no session present, then we need to subscribe, but if
        // there is a session, then the server remembers us and our
        // subscriptions.
        /*
        if (!rsp.is_session_present())
            client->subscribe(STATUS_TOPIC, QOS)->wait();
        auto top = mqtt::topic(client, STATUS_TOPIC, QOS);*/
        

        cout << "Publishing for messages on topic: '" << TOPIC << "'" << endl;
        while (true) {
            this_thread::sleep_for(DURATION);
            //TODO here were parse the messages, from msg->to_string()
            string message = "{{'temperature' : " + to_string(Fridge::getTemperature()) + "} };"  ;
            const char *payload = message.c_str();
            client->publish(STATUS_TOPIC, payload, strlen(payload) + 1, 0, false);
            //top.publish("{{'temperature' : " + to_string(Fridge::retTemperature()) + "} };"  ;
        }

        // If we're here, the client was almost certainly disconnected.
        // But we check, just to make sure.

        if (client->is_connected()) {
            cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
            client->disconnect()->wait();
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