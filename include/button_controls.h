#ifndef BUTTON_CONTROLS_H
#define BUTTON_CONTROLS_H

#include "component.h"

class ButtonControls: public Component
{
  public:
    void setup();
    void loop();
  
  private:
  
    static const int BTN_ACCEPT = D0;
    static const int BTN_UP     = D3;
    static const int BTN_DOWN   = D4;
  
    uint8_t stateAccept;
    uint8_t stateUp;
    uint8_t stateDown;
  
    unsigned long eventTime;
};

extern ButtonControls buttonControls;

#endif //BUTTON_CONTROLS_H
