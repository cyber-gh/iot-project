#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "SmartFridgeService.h"
#include "SmartFridgeMqttClient.h"

int main() {
    if (fork() == 0) {
        SmartFridgeService service = SmartFridgeService();
        service.start();
    } else {
        if (fork() == 0) {
            string command = "mosquitto -p " + SERVER_PORT;
            system(command.c_str()); //TODO run the broker in a different docker container
        } else {
            sleep(3);
            if (fork() == 0) {
                SmartFridgeMqttClient *client = new SmartFridgeMqttClient(SERVER_ADDRESS, CLIENT_ID);
                client->runListener();
            }
            else {
                SmartFridgeMqttClient *client = new SmartFridgeMqttClient(SERVER_ADDRESS, PUBLISHER_ID);
                client->runPublisher();
            }
            
        }
    }

}