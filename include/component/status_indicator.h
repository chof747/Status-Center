#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include "component.h"
#include "component/mqtt/mqtt_subscriber.h"
#include <Arduino.h>

class StatusIndicator: public Component, MqttSubscriber
{
    public:

        static const int LED_CRITICAL = D0;
        static const int LED_WARNING  = D5;
        static const int LED_NOMINAL  = D6;
        static const int LED_INDICAT  = D7;

        void onMessage(String topic, String payload);
        callback_t getCallback();
        
        void setup();
        void afterSetup();
        void loop();

        void switchIndicator(bool enable);
        void switchIndicator(bool enable, unsigned long duration);
        void switchAllOff();
        void turnOn(const int led);
        void turnOff(const int led);
        void toggle(const int led);

    private:

        unsigned long stateStart;
        unsigned long stateDuration;

        unsigned long indicationStart;
        unsigned long indicationDuration;

        void handleStatusIndicatorMessage(String message);
        void switchState(const char* status);
};

extern StatusIndicator statusIndicator;

#endif //STATUS_INDICATOR_H