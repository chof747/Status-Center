#include "controller/idle_controller.h"
#include "component/button_controls.h"
#include "logger.h"
#include "config.h"

#include "component/display.h"
#include "component/status_indicator.h"

#define MODULE "IDLE"

IdleController::IdleController(ButtonControls *buttons, activation_cb_t cb) : ControllerBase(cb), ButtonController(buttons), TimeOutController(CONTROLLER_TIMEOUT / 5),
                                                                              message(false), ledsActive(true)
//*********************************************************************************
{
}

void IdleController::activate()
//*********************************************************************************
{
  Log.debug(MODULE, "Idle Controller activated");
  attach();

  printMessage();
  ledActivation();
}

void IdleController::loop()
//*********************************************************************************
{
  if (checkTimeForTimeOut() && message)
  {
    display.clear();
    message = false;
  }
}

void IdleController::onClick(uint8_t state)
//*********************************************************************************
{
  if (ButtonControls::BTN_ACCEPT == state)
  {
    gotoNext();
  }
  else if (ButtonControls::BTN_DOWN == state)
  {
    toggleLedActivation();
  }
}

void IdleController::printMessage()
//*********************************************************************************
{
  display.clear();
  display.printtoinv(1, "Idle mode ...");
  display.printto(2, "Ligthts %s", (ledsActive) ? "on" : "off");

  message = true;
  restartTimer();
}

void IdleController::ledActivation()
//*********************************************************************************
{
  statusIndicator.enableLeds(ledsActive);
}

void IdleController::toggleLedActivation()
//*********************************************************************************
{
  ledsActive = !ledsActive;
  ledActivation();
  printMessage();
}
