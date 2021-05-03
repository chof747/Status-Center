#include "button_controls.h"
#include <Arduino.h>
#include "logger.h"

#define ACCEPT_OFF  LOW
#define ACCEPT_ON   HIGH
#define UP_OFF      HIGH
#define UP_ON       LOW
#define DOWN_OFF    HIGH
#define DOWN_ON     LOW

#define MODULE "BUTTONS"

void ButtonControls::setup()
//*********************************************************************************
{
  pinMode(ButtonControls::BTN_ACCEPT, INPUT_PULLDOWN);
  digitalWrite(ButtonControls::BTN_ACCEPT, ACCEPT_OFF);
  Log.debug(MODULE, "Defined Accept Button on GPIO %d", ButtonControls::BTN_ACCEPT);
  
  pinMode(ButtonControls::BTN_UP, INPUT_PULLUP);
  digitalWrite(ButtonControls::BTN_UP, UP_OFF);
  Log.debug(MODULE, "Defined Up Button on GPIO %d", ButtonControls::BTN_UP);
  
  pinMode(ButtonControls::BTN_DOWN, INPUT_PULLUP);
  digitalWrite(ButtonControls::BTN_DOWN, DOWN_OFF);
  Log.debug(MODULE, "Defined Down Button on GPIO %d", ButtonControls::BTN_DOWN);
  
  stateAccept = ACCEPT_OFF;
  stateUp = UP_OFF;
  stateDown = DOWN_OFF;
  
  eventTime = 0;
}

void ButtonControls::loop()
//*********************************************************************************
{
  
}

ButtonControls buttonControls;
