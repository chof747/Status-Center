#include "controller/message_controller.h"
#include "controller/response_controller.h"
#include "component/button_controls.h"
#include "logger.h"
#include "component/display.h"
#include "config.h"


#define MODULE "MSG_CTRL"


MessageController::MessageController(ButtonControls* buttons, ResponseController* responding, ControllerBase* timeout, activation_cb_t cb)
:ControllerBase(cb), ButtonController(buttons), TimeOutController(CONTROLLER_TIMEOUT),
responding(responding), timeoutTarget(timeout), api(MESSAGE_API_ENDPOINT), msg(EMPTY_MSG)
//*********************************************************************************
{
    
}

void MessageController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Message Controller activated");
    attach();
    restartTimer();
    msg = api.firstMessage();
}

void MessageController::loop()
//*********************************************************************************
{
    if (0 != msg.title.compareTo(""))
    {
        display.printtoinv(1, msg.title.c_str());
        display.printwrap(2, msg.message.c_str());
        msg = EMPTY_MSG;
    }

    if (checkTimeForTimeOut()) 
    {
        gotoController(timeoutTarget);
    }
}

void MessageController::onClick(uint8_t state)
//*********************************************************************************
{
   restartTimer();
   switch(state)
    {
        case ButtonControls::BTN_ACCEPT:
        if (api.hasResponseOptions()) {
            responding->setApi(&api);
            gotoAlternateNext();
        } else {
            api.acceptMessage();
            msg = api.firstMessage();
        }
        break;  

        case ButtonControls::BTN_UP:
        displayPrevMessage();
        break;

        case ButtonControls::BTN_DOWN:
        displayNextMessage();
        break;
    }

}

void MessageController::onLongPress(uint8_t state)
//*********************************************************************************
{
    restartTimer();
    if (ButtonControls::BTN_ACCEPT == state)
    {
        gotoNext();
    }
}

void MessageController::displayNextMessage()
//*********************************************************************************
{
    Log.debug(MODULE, "Displaying next message");
    msg = api.nextMessage();

}

void MessageController::displayPrevMessage()
//*********************************************************************************
{
    Log.debug(MODULE, "Displaying previous message");
    msg = api.prevMessage();
}