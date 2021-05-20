#ifndef BUTTON_CONTROLS_H
#define BUTTON_CONTROLS_H

#include <Arduino.h>
#include "component/component.h"

class ButtonController;

class ButtonControls: public Component
{
  public:
    void setup();
    void loop();

    void setController(ButtonController* controller);

    static const int BTN_ACCEPT = 0x01;
    static const int BTN_UP     = 0x02;
    static const int BTN_DOWN   = 0x04;

  private:
  
    static const int PIN_ACCEPT = D8;
    static const int PIN_UP     = D3;
    static const int PIN_DOWN   = D4;
  
    uint8_t state;
    uint8_t eventState;
    unsigned long eventTime;
    uint8_t clicks;
    bool longPress;

    ButtonController* controller;

    uint8_t readButtons();
};

extern ButtonControls buttonControls;

#endif //BUTTON_CONTROLS_H
