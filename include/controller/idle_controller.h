#ifndef IDLE_CONTROLLER_H
#define IDLE_CONTROLLER_H

#include "controller/controller.h"
#include "controller/button_controller.h"

class ButtonControls;

class IdleController: public Controller, public ButtonController
{
    public:

    IdleController(ButtonControls* buttons, activation_cb_t cb);

    void activate();
    void loop();

    void onClick(uint8_t state);

    private:

    ButtonControls* buttons;
};

#endif //IDLE_CONTROLLER_H