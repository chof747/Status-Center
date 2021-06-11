#ifndef RESPONSE_CONTROLLER_H
#define RESPONSE_CONTROLLER_H

#include "./base.h"
#include "./button_controller.h"
#include <String.h>
#include "./message/message_api.h"

class ResponseController: public ControllerBase, ButtonController
{
    public:

    ResponseController(ButtonControls* buttons, activation_cb_t cb);
    void setApi(MessageApi* api) { this->api = api; }

    void activate();
    void loop();

    void onClick(uint8_t state);

    private:

    MessageApi *api;
    response_t response;
    void displayNextResponse();
    void displayPrevResponse();
};

#endif //RESPONSE_CONTROLLER_H

