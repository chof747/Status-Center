#include "controller/idle_controller.h"
#include "component/button_controls.h"
#include "logger.h"

#define MODULE "IDLE"

IdleController::IdleController(ButtonControls* buttons, activation_cb_t cb): 
  ControllerBase(cb), ButtonController(buttons)
//*********************************************************************************
{
}

void IdleController::activate()
//*********************************************************************************
{
    attach();    
}

void IdleController::loop()
//*********************************************************************************
{

}

void IdleController::onClick(uint8_t state)
//*********************************************************************************
{
    Log.info(MODULE, "Activating next controller from idle state!");
    gotoNext();
}