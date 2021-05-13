#include "controller/controller.h"

Controller::Controller(activation_cb_t cb): 
  next(NULL), alternateNext(NULL), activationCallBack(cb) {}

void Controller::setNext(Controller* c)
//****************************************************************************************
{
    this->next = c;
}

void Controller::setAlternateNext(Controller* c)
//****************************************************************************************
{
    this->alternateNext = c;
}

void Controller::gotoNext()
//****************************************************************************************
{
    if (NULL != next)
    {
        next->activate();
        activationCallBack(next);
    }
}

void Controller::gotoAlternateNext()
//****************************************************************************************
{
    if (NULL != alternateNext)
    {
        alternateNext->activate();
        activationCallBack(next);
    }
}