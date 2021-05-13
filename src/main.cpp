#include <Arduino.h>
#include "logger.h"
#include "wifi.h"
#include "mqtt.h"
#include "status_indicator.h"
#include "button_controls.h"
#include "display.h"

#include "controller/controller.h"
#include "controller/idle_controller.h"

#define MAX_COMPONENTS  5

Component* components[MAX_COMPONENTS];

Controller* activeController = NULL;

void onNextController(Controller* active)
{
  activeController = active;
}

IdleController idle(&buttonControls, &onNextController);

void setup() 
//****************************************************************************************
{
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &wifiManager;
  components[1] = &mqttClient;
  components[2] = &statusIndicator;
  components[3] = &display;
  components[4] = &buttonControls;

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->setup();
  }

  activeController = &idle;
  activeController->activate();
}

void loop() 
//****************************************************************************************
{
  // put your main code here, to run repeatedly:
  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->loop();
  }

  //do any work in the currently active controller
  if (NULL != activeController)
  {
    activeController->loop();
  }

  delay(10);
}