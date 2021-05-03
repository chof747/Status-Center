#include "status_indicator.h"
#include "mqtt.h"
#include "logger.h"

#define MODULE "STATUS-LED"


void StatusIndicator::setup()
{
    mqttClient.subscribe("alarmcenter/statusindicator", getCallback());
}

void StatusIndicator::loop()
{

}

callback_t StatusIndicator::getCallback()
{
    return CALLBACK_TRANSLATOR(StatusIndicator);
}

void StatusIndicator::onMessage(String topic, String message)
{
    Log.info(MODULE,"Command: %s", topic.c_str());
    Log.info(MODULE,"Received: %s", message.c_str());
}

StatusIndicator statusIndicator;