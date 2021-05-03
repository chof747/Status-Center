#include <Arduino.h>
#include "logger.h"
#include "wifi.h"
#include "mqtt.h"
#include "status_indicator.h"

#define MAX_COMPONENTS  3

Component* components[MAX_COMPONENTS];


void setup() 
//****************************************************************************************
{
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &wifiManager;
  components[1] = &mqttClient;
  components[2] = &statusIndicator;

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->setup();
  }

}

void loop() 
//****************************************************************************************
{
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->loop();
  }
  delay(10);
}