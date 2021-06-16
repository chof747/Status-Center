#ifndef MESSAGE_CONTROLLER_H
#define MESSAGE_CONTROLLER_H

#include "./base.h"
#include "./button_controller.h"
#include "./timeout_controller.h"
#include <String.h>
#include "./message/message_api.h"

class ResponseController;

class MessageController: public ControllerBase, ButtonController, TimeOutController
{
    public:

    MessageController(ButtonControls* buttons, ResponseController* responding, ControllerBase* timeout, activation_cb_t cb);
    void activate();
    void loop();

    void onClick(uint8_t state);
    void onLongPress(uint8_t state);

    private:

    ResponseController* responding;
    ControllerBase* timeoutTarget;
    MessageApi api;
    message_t msg;
    void displayNextMessage();
    void displayPrevMessage();
};

#endif //MESSAGE_CONTROLLER_H

