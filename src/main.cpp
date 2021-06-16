#include <Arduino.h>
#include "logger.h"
#include "component/wifi.h"
#include "component/mqtt.h"
#include "component/status_indicator.h"
#include "component/button_controls.h"
#include "component/display.h"

#include "controller/base.h"
#include "controller/idle_controller.h"
#include "controller/message_controller.h"
#include "controller/response_controller.h"

#define MAX_COMPONENTS  5

Component* components[MAX_COMPONENTS];

ControllerBase* activeController = NULL;

void onNextController(ControllerBase* active)
//****************************************************************************************
{
  activeController = active;
}

IdleController idle(&buttonControls, &onNextController);
ResponseController response(&buttonControls, &onNextController);
MessageController message(&buttonControls, &response, &idle, &onNextController);

void setup() 
//****************************************************************************************
{
  // put your setup code here, to run once:
  Log.info("MAIN", "Starting");

  components[0] = &statusIndicator;
  components[1] = &wifiManager;
  components[2] = &mqttClient;
  components[3] = &display;
  components[4] = &buttonControls;

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    if (components[i]->powerOnTest())
    {
      components[i]->setup();
    }
  }

  for(int i=0;i<MAX_COMPONENTS;++i)
  {
    components[i]->afterSetup();
  }

  idle.setNext(&message);
  message.setNext(&idle);
  message.setAlternateNext(&response);
  response.setNext(&message);

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