#ifndef BUTTON_CONTROLS_H
#define BUTTON_CONTROLS_H

#include <Arduino.h>
#include "component.h"

class ButtonControls: public Component
{
  public:
    void setup();
    void loop();
  
  private:
  
    static const int BTN_ACCEPT = D8;
    static const int BTN_UP     = D3;
    static const int BTN_DOWN   = D4;
  
    uint8_t state;
    uint8_t eventState;
    unsigned long eventTime;
    uint8_t clicks;
    bool longPress;

    uint8_t readButtons();
};

extern ButtonControls buttonControls;

#endif //BUTTON_CONTROLS_H
