#include <Arduino.h>
#include "logger.h"
#include "wifi.h"
#include "mqtt.h"

#define MAX_COMPONENTS  2

Component* components[MAX_COMPONENTS];


void setup() {
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &wifiManager;
  components[1] = &mqttClient;

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->setup();
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->loop();
  }
  delay(10);
}