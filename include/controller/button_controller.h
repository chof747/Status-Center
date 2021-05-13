#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

class ButtonController
{
    public:

    virtual void onClick(uint8_t state) {}
    virtual void onLongPress(uint8_t state) {}
    virtual void onDblClick(uint8_t state) {}
};


#endif //BUTTON_CONTROLLER_H