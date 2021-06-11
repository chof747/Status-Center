#include "controller/base.h"

ControllerBase::ControllerBase(activation_cb_t cb): 
  next(NULL), alternateNext(NULL), activationCallBack(cb) {}

void ControllerBase::setNext(ControllerBase* c)
//****************************************************************************************
{
    this->next = c;
}

void ControllerBase::setAlternateNext(ControllerBase* c)
//****************************************************************************************
{
    this->alternateNext = c;
}

void ControllerBase::gotoNext()
//****************************************************************************************
{
    if (NULL != next)
    {
        next->activate();
        activationCallBack(next);
    }
}

void ControllerBase::gotoAlternateNext()
//****************************************************************************************
{
    if (NULL != alternateNext)
    {
        alternateNext->activate();
        activationCallBack(alternateNext);
    }
}