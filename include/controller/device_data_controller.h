#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include "./base.h"
#include "./button_controller.h"
#include "./timeout_controller.h"
#include <String.h>
#include "./message/message_api.h"

class DeviceDataController: public ControllerBase, ButtonController, TimeOutController
{
    public:

    DeviceDataController(ButtonControls* buttons, ControllerBase* timeout, activation_cb_t cb);
    void activate();
    void loop();

    void onClick(uint8_t state);

    private:

    void obtainDeviceData(uint8_t ix);

    ControllerBase* timeoutTarget;
    String label;
    String value;
    uint8_t index;
};

#endif //DEVICE_CONTROLLER_H

