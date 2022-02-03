#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

#include "component.h"
#include "component/mqtt/mqtt_subscriber.h"
#include <Arduino.h>

class StatusIndicator: public Component, MqttSubscriber
{
    public:

        static const int STATE_CRITICAL = 1;
        static const int STATE_WARNING  = 2;
        static const int STATE_NOMINAL  = 4;

        void onMessage(String topic, String payload);
        callback_t getCallback();
        
        void setup();
        void afterSetup();
        void loop();

        void display();

        void switchIndicator(bool enable);
        void switchIndicator(bool enable, unsigned long duration);
        void switchAllOff();
        void switchTo(const int state);
        void enableLeds(bool enable);

    private:

        static const int LED_CRITICAL = D5;
        static const int LED_WARNING  = D6;
        static const int LED_NOMINAL  = D7;
        static const int LED_INDICAT  = D0;
        
        unsigned long stateStart;
        unsigned long stateDuration;

        unsigned long indicationStart;
        unsigned long indicationDuration;

        unsigned int state;
        bool indicator;
        bool enabled;

        void handleStatusIndicatorMessage(String message);
        void switchState(const char* status);
        void checkTiming();
        void toggleLeds();
        void turnOff();
};

extern StatusIndicator statusIndicator;

#endif //STATUS_INDICATOR_H