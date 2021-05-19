#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include <Arduino.h>

class ButtonControls;

class ButtonController
{
    public:

    ButtonController(ButtonControls* buttons);

    virtual void onClick(uint8_t state) {}
    virtual void onLongPress(uint8_t state) {}
    virtual void onDblClick(uint8_t state) {}

    protected:

    void attach();

    private:

    ButtonControls* buttons;
};


#endif //BUTTON_CONTROLLER_H