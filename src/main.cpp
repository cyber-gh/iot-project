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
            system("mosquitto"); //TODO run the broker in a different docker container
        } else {
            SmartFridgeMqttClient *client = new SmartFridgeMqttClient();
            client->runListener();
        }
    }

}