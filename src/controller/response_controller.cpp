#include "controller/response_controller.h"
#include "component/button_controls.h"
#include "logger.h"

#include "component/display.h"
#include "component/status_indicator.h"

#define MODULE "RESP_CTRL"

#ifndef ACTION_HEADER
#define ACTION_HEADER "Possible Fix"
#endif

ResponseController::ResponseController(ButtonControls *buttons, activation_cb_t cb)
    : ControllerBase(cb), ButtonController(buttons), 
    api(NULL), response(EMPTY_RESPONSE), currentResponseId(NULL)
//*********************************************************************************
{
}

void ResponseController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Response Controller activated");
    attach();
    response = api->firstResponse();
    currentResponseId = response.id;
    Log.debug(MODULE, "id: %s, label: \"%s\"", response.id, response.label);
}

void ResponseController::loop()
//*********************************************************************************
{
    if (response.id != NULL)
    {
        display.printtoinv(1, ACTION_HEADER);
        display.printwrap(2, response.label);
        response = EMPTY_RESPONSE;
    }
}

void ResponseController::onClick(uint8_t state)
//*********************************************************************************
{
    switch (state)
    {
    case ButtonControls::BTN_ACCEPT:
        if (NULL != currentResponseId) {
            api->acceptMessage(currentResponseId);
        } else {
            api->acceptMessage();
        }
        statusIndicator.switchIndicator(true, 1);
        gotoNext();
        break;

    case ButtonControls::BTN_UP:
        displayPrevResponse();
        break;

    case ButtonControls::BTN_DOWN:
        displayNextResponse();
        break;
    }
}

void ResponseController::onLongPress(uint8_t state)
//*********************************************************************************
{
    if (ButtonControls::BTN_DOWN == state)
    {
        Log.info(MODULE, "Do not accept message - returning to controller");
        gotoNext();
    }

    if (ButtonControls::BTN_ACCEPT == state)
    {
        api->acceptMessage();
        statusIndicator.switchIndicator(true, 1);
        gotoNext();
    }
}

void ResponseController::displayNextResponse()
//*********************************************************************************
{
    Log.debug(MODULE, "Displaying next response");
    response = api->nextResponse();
    currentResponseId = response.id;
}

void ResponseController::displayPrevResponse()
//*********************************************************************************
{
    Log.debug(MODULE, "Displaying previous response");
    response = api->prevResponse();
    currentResponseId = response.id;
}