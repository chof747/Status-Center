#include "button_controls.h"
#include "logger.h"

#define MODULE "BUTTONS"

#define DEBOUNCE_TIME 20
#define LONG_PRESS_TIME 1000
#define DBL_CLICK_TIME 110

void ButtonControls::setup()
//*********************************************************************************
{
  pinMode(ButtonControls::BTN_ACCEPT, INPUT);
  digitalWrite(ButtonControls::BTN_ACCEPT, LOW);
  Log.debug(MODULE, "Defined Accept Button on GPIO %d", ButtonControls::BTN_ACCEPT);
  
  pinMode(ButtonControls::BTN_UP, INPUT_PULLUP);
  digitalWrite(ButtonControls::BTN_UP, HIGH);
  Log.debug(MODULE, "Defined Up Button on GPIO %d", ButtonControls::BTN_UP);
  
  pinMode(ButtonControls::BTN_DOWN, INPUT_PULLUP);
  digitalWrite(ButtonControls::BTN_DOWN, HIGH);
  Log.debug(MODULE, "Defined Down Button on GPIO %d", ButtonControls::BTN_DOWN);
  
  state = 0;  
  eventState = 0;
  eventTime = 0;
  clicks = 0;
}

void ButtonControls::loop()
//*********************************************************************************
{
  uint8_t newState = readButtons();
  if (newState != state)
  {
    //Log.debug(MODULE, "New State detected: %02X (old was %02X) at %d", newState, state, millis());
    if (newState > state)
    {
      //button has been pressed start 
      eventTime = millis();
      state = newState;
      eventState = state;
    }  
    else if (newState == 0)
    {
      //release
      unsigned long pressTime = millis() - eventTime;
      //Log.debug(MODULE,"Click counter before = %d", clicks);
      clicks += 1;

      if (LONG_PRESS_TIME <= pressTime)
      {
        eventTime = 0;
        longPress = true;
      }
      else if ((LONG_PRESS_TIME > pressTime) && (DEBOUNCE_TIME <= pressTime))
      {
        longPress = false;
        eventTime = (clicks > 1) ? 0 : millis();
      }
      else 
      {
        //debouncing
      }

      state = newState;
      //Log.debug(MODULE,"Click counter after = %d", clicks);
    }
  }

  //  Log.debug(MODULE, "Time passed since last %02X click = %d", eventState, (millis() - eventTime));

  if ((DBL_CLICK_TIME < (millis() - eventTime)) && (clicks > 0))
  {
      if (longPress)
      {
        Log.debug(MODULE, "Long press of button state x%02X", eventState);
      }
      else if (2 == clicks)
      {
        Log.debug(MODULE, "Double Click of button state x%02X", eventState);
      }
      else 
      {
        Log.debug(MODULE, "Click of button state x%02X", eventState);
      }

      clicks = 0;
      eventTime = 0;
      eventState = 0;
  }
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
  return digitalRead(ButtonControls::BTN_ACCEPT) 
         | !digitalRead(ButtonControls::BTN_UP) << 1 
         | !digitalRead(ButtonControls::BTN_DOWN) << 2; 
  
}

ButtonControls buttonControls;
