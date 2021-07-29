#include "controller/idle_controller.h"
#include "component/button_controls.h"
#include "logger.h"

#include "component/display.h"

#define MODULE "IDLE"

IdleController::IdleController(ButtonControls* buttons, activation_cb_t cb): 
  ControllerBase(cb), ButtonController(buttons)
//*********************************************************************************
{
}

void IdleController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Idle Controller activated");
    attach();    
    display.clear();
}

void IdleController::loop()
//*********************************************************************************
{

}

void IdleController::onClick(uint8_t state)
//*********************************************************************************
{
  if (ButtonControls::BTN_ACCEPT == state) gotoNext();
}