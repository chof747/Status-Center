#include "controller/button_controller.h"
#include "component/button_controls.h"

ButtonController::ButtonController(ButtonControls* buttons)
//*********************************************************************************
{
    this->buttons = buttons;
}

void ButtonController::attach()
//*********************************************************************************
{
    buttons->setController(this);
}