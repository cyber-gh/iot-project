
//
// Created by Soltan Gheorghe on 16.03.2021.
//

#include "SmartFridgeMqttClient.h"
#include "models.h"

using json = nlohmann::json;

uint64_t timestamp()
{
    auto now = system_clock::now();
    auto tse = now.time_since_epoch();
    auto msTm = duration_cast<milliseconds>(tse);
    return uint64_t(msTm.count());
}

string SmartFridgeMqttClient::trim_input_message(string msg) {

    string message;
    for (int i = 0; i < msg.size() - 3; ++i) {
        if (i == 1 || i == 2 || i == 3) {
            continue;
        }
        message += msg[i];
    }
    return message;
}

void SmartFridgeMqttClient::process_voltage(string message) {
    try {
        int value = stoi(message);
        if (value > 100) {
            return;
        }
        bool b = Fridge::setEcoMode();
        if (b == false) {
            cout << "Error in set eco mode\n";
            throw ;
        }
        cout << "Low voltage. Setting fridge to eco mode\n";
    }
    catch (...) {
        cout << "Error in processing voltage\n";
    }

}

void SmartFridgeMqttClient::process_message(string message) {
    DatabaseAccess db = DatabaseAccess::getInstance();
    try {
        InputStructure is;
        auto jsonValue = json::parse(message);
        from_json(jsonValue, is);

        if (is.status == "DELETE") {
            Search searcher = Search();
            auto query = searcher.genDeleteProductQuery(is.value);
            db.executeQuery(query);
        } else if (is.status == "SETFRIDGE") {
            int temp = stoi(is.value);
            string query = Fridge::setTempQuery(temp);
            db.executeQuery(query);
        }
    } catch(std::exception& exc) { }
    try{
        Product product;
        auto jsonValue = json::parse(message);
        from_json(jsonValue, product);
        auto query = product.genInsertQuery();

        db.executeQuery(query);
    } catch (std::exception& exc) { }
    try {
        IncreaseQuantityStructure s;
        auto jsonValue = json::parse(message);
        from_json(jsonValue, s);
        auto query = Fridge::increaseQuantityQuery(s.productName, s.quantity);
        db.executeQuery(query);
    } catch (std::exception& exc) { }
}

void SmartFridgeMqttClient::runListener() {
    auto connOpts = mqtt::connect_options_builder()
            .clean_session(false)
            .finalize();

    try {
        client->start_consuming();
        cout << "Client Connecting to the MQTT server..." << flush;
        auto tok = client->connect(connOpts);

        auto rsp = tok->get_connect_response();

        if (!rsp.is_session_present()) {
            client->subscribe(TOPIC, QOS)->wait();
            client->subscribe(VOLTAGE_TOPIC, QOS)->wait();
        }
        cout << "OK - Client Connected" << endl;

        cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;
        while (true) {
            auto msg = client->consume_message();
            if (!msg) break;
            if (msg->get_topic() == VOLTAGE_TOPIC) {
                process_voltage(msg->to_string());
                continue;
            }
            //TODO here were parse the messages, from msg->to_string()
            cout << msg->get_topic() << ": " << msg->to_string() << endl;
//            string message = trim_input_message(msg->to_string());
            string message = msg -> to_string();
            process_message(message);
        }

        if (client->is_connected()) {
            cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
            client->unsubscribe(TOPIC)->wait();
            client->stop_consuming();
            client->disconnect()->wait();
        }
        else {
            cout << "\nClient was disconnected" << endl;
        }
    }
    catch (const mqtt::exception& exc) {
        cerr << "\nSomething went terribly wrong with the client\n  " << exc << endl;
        return ;
    }
}

void SmartFridgeMqttClient::runPublisher() {
    uint64_t trun = 0;

    auto createOpts = mqtt::create_options_builder()
            .send_while_disconnected(true, true)
            .max_buffered_messages(MAX_BUFFERED_MESSAGES)
            .delete_oldest_messages()
            .finalize();

    mqtt::async_client cli(SERVER_ADDRESS, "", createOpts);

    cli.set_connected_handler([&cli](const std::string&) {
        std::cout << "*** Publisher Connected ("
                  << timestamp() << ") ***" << std::endl;
    });

    cli.set_connection_lost_handler([&cli](const std::string&) {
        std::cout << "*** Publisher Connection Lost ("
                  << timestamp() << ") ***" << std::endl;
    });

    auto willMsg = mqtt::message("test/events", "Time publisher disconnected", 1, true);
    auto connOpts = mqtt::connect_options_builder()
            .clean_session()
            .will(willMsg)
            .automatic_reconnect(seconds(1), seconds(10))
            .finalize();

    try {
        cout << "Starting Publisher connection..." << endl;
        cli.connect(connOpts);

        auto top = mqtt::topic(cli, STATUS_TOPIC, QOS);
        cout << "Publishing data..." << endl;

        while (timestamp() % DELTA_MS != 0);

        uint64_t	t = timestamp(),
                tlast = t,
                tstart = t;

        top.publish(to_string(t));

        while (true) {
            this_thread::sleep_for(SAMPLE_PERIOD);

            t = timestamp();

            if (abs(int(t - tlast)) >= DELTA_MS) {

                string message = "{{'temperature' : " + to_string(Fridge::getTemperature()) + "} };"  ;
                const char *payload = message.c_str();
                top.publish(payload);

                cout << "Publishing message: " << endl;
                cout << message << endl;

                vector<string> productsVec = Fridge::getAllProductsNames();
                string products;
                for (auto product : productsVec) products += product + ", ";

                message = "{{'product_name' : [" + products + "]}};" ;
                const char *payload2 = message.c_str();
                top.publish(payload2);

                cout << "Publishing message: " << endl;
                cout << message << endl;
            }

            if (trun > 0 && t >= (trun + tstart))
                break;
        }

        // Disconnect
        cout << "\nDisconnecting..." << endl;
        cli.disconnect()->wait();
        cout << "  ...OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
        return;
    }

}