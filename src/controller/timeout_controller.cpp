#include "controller/timeout_controller.h"

#include <Arduino.h>

TimeOutController::TimeOutController(unsigned long timeout)
//*********************************************************************************
{
    this->timeout = timeout;
    restartTimer();
}

void TimeOutController::restartTimer()
//*********************************************************************************
{
    referenceTime = millis();
}

bool TimeOutController::checkTimeForTimeOut()
//*********************************************************************************
{
    return ((millis() - referenceTime) > timeout);
}