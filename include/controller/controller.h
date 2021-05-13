#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

class Controller;
typedef void(*activation_cb_t)(Controller* active);

class Controller 
{
    public:
    
    Controller(activation_cb_t cb);
    
    virtual void activate() = 0;
    virtual void loop() = 0;
    virtual void deactivate() {}

    void setNext(Controller* c);
    void setAlternateNext(Controller* c);


    protected:

    void gotoNext();
    void gotoAlternateNext();

    private:

    Controller* next;
    Controller* alternateNext;

    activation_cb_t activationCallBack;

};

#endif //CONTROLLER_H