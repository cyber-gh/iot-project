#include "SmartFridgeService.h"
#include "SmartFridgeMqttClient.h"

int main() {

//    SmartFridgeService service = SmartFridgeService();
//    service.start();

    SmartFridgeMqttClient client = SmartFridgeMqttClient();

    client.run();
}