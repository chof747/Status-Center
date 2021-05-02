#include "status_indicator.h"
#include "mqtt.h"

void StatusIndicator::setup()
{
    MqttClient.subscribe("")
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

}