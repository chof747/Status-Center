#include "component/status_indicator.h"
#include "component/mqtt.h"
#include "logger.h"

#include <ArduinoJson.h>

#define MODULE "STATUS-LED"

#define STANDARD_STATE "nominal"

void StatusIndicator::setup()
//****************************************************************************************
{
    mqttClient.subscribe("alarmcenter/statusindicator", getCallback());
    mqttClient.subscribe("alarmcenter/indicator", getCallback());

    pinMode(StatusIndicator::LED_CRITICAL, OUTPUT);
    pinMode(StatusIndicator::LED_WARNING, OUTPUT);
    pinMode(StatusIndicator::LED_NOMINAL, OUTPUT);
    pinMode(StatusIndicator::LED_INDICAT, OUTPUT);

    digitalWrite(StatusIndicator::LED_CRITICAL, LOW);
    digitalWrite(StatusIndicator::LED_WARNING, LOW);
    digitalWrite(StatusIndicator::LED_NOMINAL, LOW);
    digitalWrite(StatusIndicator::LED_INDICAT, LOW);

    stateStart = 0;
    stateDuration = 0;
    switchState(STANDARD_STATE);
}

void StatusIndicator::loop()
//****************************************************************************************
{
    if ((0 != stateDuration) && (stateDuration < (millis() - stateStart) / 1000))
    {
        switchState(STANDARD_STATE);
        stateStart = 0;
        stateDuration = 0;
    }
}

callback_t StatusIndicator::getCallback()
//****************************************************************************************
{
    return CALLBACK_TRANSLATOR(StatusIndicator);
}

void StatusIndicator::onMessage(String topic, String message)
//****************************************************************************************
{
    if (topic.compareTo("/alarmcenter/statusindicator") == 0)
    {
        handleStatusIndicatorMessage(message);
    } 
    else if (topic.compareTo("/alarmcenter/indicator") == 0)
    {
        message.toUpperCase();
        switchIndicator(message.compareTo("ON") == 0);
    }
}

void StatusIndicator::switchIndicator(bool enable)
//****************************************************************************************
{
    digitalWrite(StatusIndicator::LED_INDICAT, (enable) ? HIGH : LOW);
}

void StatusIndicator::handleStatusIndicatorMessage(String message)
//****************************************************************************************
{
    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, message);
    if (!error)
    {
        switchState(doc["status"]);
        stateDuration = doc["expires"];
        stateStart = millis();
    }
    else
    {
        Log.warn(MODULE, "Wrongly formatted status message. Error was: %s", error.f_str());
    }
}

void StatusIndicator::switchState(const char *status)
//****************************************************************************************
{
    if (strcmp("critical", status) == 0)
    {
        digitalWrite(StatusIndicator::LED_CRITICAL, HIGH);
        digitalWrite(StatusIndicator::LED_WARNING, LOW);
        digitalWrite(StatusIndicator::LED_NOMINAL, LOW);
    }
    else if (strcmp("warning", status) == 0)
    {
        digitalWrite(StatusIndicator::LED_CRITICAL, LOW);
        digitalWrite(StatusIndicator::LED_WARNING, HIGH);
        digitalWrite(StatusIndicator::LED_NOMINAL, LOW);
    }
    else if (strcmp("nominal", status) == 0)
    {
        digitalWrite(StatusIndicator::LED_CRITICAL, LOW);
        digitalWrite(StatusIndicator::LED_WARNING, LOW);
        digitalWrite(StatusIndicator::LED_NOMINAL, HIGH);
    }

    Log.info(MODULE, "State set to %s", status);
}

StatusIndicator statusIndicator;