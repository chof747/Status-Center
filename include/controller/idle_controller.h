#ifndef IDLE_CONTROLLER_H
#define IDLE_CONTROLLER_H

#include "controller/base.h"
#include "controller/button_controller.h"
#include "controller/timeout_controller.h"

class ButtonControls;

class IdleController: public ControllerBase, public ButtonController, TimeOutController
{
    public:

    IdleController(ButtonControls* buttons, activation_cb_t cb);

    void activate();
    void loop();

    void onClick(uint8_t state);

    private:
    
    bool message;
    bool ledsActive;

    void printMessage();
    void ledActivation();
    void toggleLedActivation();

};

#endif //IDLE_CONTROLLER_H