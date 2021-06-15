#include "component/status_indicator.h"
#include "component/mqtt.h"
#include "logger.h"

#include <ArduinoJson.h>

#define MODULE "STATUS-LED"

#define STANDARD_STATE "nominal"

void StatusIndicator::setup()
//****************************************************************************************
{
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
    indicationStart = 0;
    indicationDuration = 0;
    turnOn(StatusIndicator::LED_CRITICAL);
}

void StatusIndicator::afterSetup()
//****************************************************************************************
{
    mqttClient.subscribe("alarmcenter/statusindicator", getCallback());
    mqttClient.subscribe("alarmcenter/indicator", getCallback());
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

    if ((0 != indicationDuration) && (indicationDuration < (millis() - indicationStart) / 1000 ))
    {
        switchIndicator(false);
        indicationStart = 0;
        indicationDuration = 0;
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

void StatusIndicator::switchIndicator(bool enable, unsigned long duration)
//****************************************************************************************
{
    digitalWrite(StatusIndicator::LED_INDICAT, (enable) ? HIGH : LOW);
    indicationStart = millis();
    indicationDuration = duration;
}
void StatusIndicator::switchAllOff()
//****************************************************************************************
{
    digitalWrite(StatusIndicator::LED_CRITICAL, LOW);
    digitalWrite(StatusIndicator::LED_WARNING, LOW);
    digitalWrite(StatusIndicator::LED_NOMINAL, LOW);
    digitalWrite(StatusIndicator::LED_INDICAT, LOW);
}

void StatusIndicator::turnOn(const int led)
//****************************************************************************************
{
    digitalWrite(led, HIGH);
}

void StatusIndicator::turnOff(const int led)
//****************************************************************************************
{
    digitalWrite(led, LOW);
}

void StatusIndicator::toggle(const int led)
//****************************************************************************************
{
    digitalWrite(led, !digitalRead(led));
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