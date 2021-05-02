#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include "component.h"
#include "mqtt_subscriber.h"

class StatusIndicator: public Component, MqttSubscriber
{
    public:

        void onMessage(String topic, String payload);
        callback_t getCallback();
        
        void setup();
        void loop();
};

#endif //STATUS_INDICATOR_H