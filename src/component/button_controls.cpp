#include "component/button_controls.h"
#include "controller/button_controller.h"
#include "logger.h"

#define MODULE "BUTTONS"

#define DEBOUNCE_TIME 20
#define LONG_PRESS_TIME 500
#define DBL_CLICK_TIME 150

void ButtonControls::setup()
//*********************************************************************************
{
  controller = NULL;

  pinMode(ButtonControls::PIN_ACCEPT, INPUT);
  digitalWrite(ButtonControls::PIN_ACCEPT, LOW);
  Log.debug(MODULE, "Defined Accept Button on GPIO %d", ButtonControls::PIN_ACCEPT);
  
  pinMode(ButtonControls::PIN_UP, INPUT_PULLUP);
  digitalWrite(ButtonControls::PIN_UP, HIGH);
  Log.debug(MODULE, "Defined Up Button on GPIO %d", ButtonControls::PIN_UP);
  
  pinMode(ButtonControls::PIN_DOWN, INPUT_PULLUP);
  digitalWrite(ButtonControls::PIN_DOWN, HIGH);
  Log.debug(MODULE, "Defined Down Button on GPIO %d", ButtonControls::PIN_DOWN);
  
  state = 0;  
  eventState = 0;
  eventTime = 0;
  clicks = 0;
}

void ButtonControls::loop()
//*********************************************************************************
{
  uint8_t newState = readButtons();
  long t = millis();
  
  if ((newState != state) && (DEBOUNCE_TIME < (t - eventTime)))
  {
    Log.debug(MODULE, "New State detected: %02X (old was %02X) at %d", newState, state, t);
    if (newState > state)
    {
      //button has been pressed start 
      eventTime = t;
      state = newState;
      eventState = state;
    }  
    else if (newState == 0)
    {
      //release
      unsigned long pressTime = t - eventTime;
      Log.debug(MODULE,"Click counter before = %d", clicks);
      clicks += 1;

      if (LONG_PRESS_TIME <= pressTime)
      {
        eventTime = 0;
        longPress = true;
      }
      else
      {
        longPress = false;
        eventTime = (clicks > 1) ? 0 : t;
      }

      state = newState;
      Log.debug(MODULE,"Click counter after = %d", clicks);
    }
    else
    {
      eventTime = t;
    }

  }

  //Log.debug(MODULE, "Time passed since last %02X click = %d", eventState, (millis() - eventTime));

  if ((DBL_CLICK_TIME < (t - eventTime)) && (clicks > 0))
  {
      if (longPress)
      {
        Log.debug(MODULE, "\033[32mLong press of button state x%02X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onLongPress(eventState);
        }
      }
      else if (2 == clicks)
      {
        Log.debug(MODULE, "\033[32mDouble Click of button state x%02X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onDblClick(eventState);
        }
      }
      else 
      {
        Log.debug(MODULE, "\033[32mClick of button state x%02X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onClick(eventState);
        }

      }

      clicks = 0;
      eventTime = 0;
      eventState = 0;
  }
}

void ButtonControls::setController(ButtonController* controller)
//*********************************************************************************
{
  this->controller = controller;
}

uint8_t ButtonControls::readButtons()
//*********************************************************************************
{
  /*
   *   HOW TO ADD BUTTONS TO THE STATE
   *   ===============================
   * 
   *   - Add a new button by the OR operatore (|)
   *   - Left shift it to the position in the state byte where it should be
   *   - If it is a pulled-up button invert the output of digitalRead to achieve
   *     a LOW for off and HIGH for on
   */
  return digitalRead(ButtonControls::PIN_ACCEPT) 
         | !digitalRead(ButtonControls::PIN_UP) << 1 
         | !digitalRead(ButtonControls::PIN_DOWN) << 2; 
  
}

ButtonControls buttonControls;
