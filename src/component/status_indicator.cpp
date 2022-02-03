#include "component/status_indicator.h"
#include "component/mqtt.h"
#include "logger.h"

#include <ArduinoJson.h>

#define MODULE "STATUS-LED"

#define STANDARD_STATE STATE_NOMINAL

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

    state = 0;
    indicator = false;

    enabled = true;
    stateStart = 0;
    stateDuration = 0;
    indicationStart = 0;
    indicationDuration = 0;
    switchTo(STATE_CRITICAL);
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
    display();
}

void StatusIndicator::display()
//****************************************************************************************
{
    checkTiming();

    if (enabled)
    {
        toggleLeds();
    }
    else
    {
        turnOff();
    }
}

void StatusIndicator::checkTiming()
//****************************************************************************************
{
    if ((0 != stateDuration) && (stateDuration < (millis() - stateStart) / 1000))
    {
        state = STANDARD_STATE;
        stateStart = 0;
        stateDuration = 0;
    }

    if ((0 != indicationDuration) && (indicationDuration < (millis() - indicationStart) / 1000))
    {
        indicator = false;
        indicationStart = 0;
        indicationDuration = 0;
    }
}

void StatusIndicator::toggleLeds()
//****************************************************************************************
{
    digitalWrite(StatusIndicator::LED_CRITICAL, (state & STATE_CRITICAL) ? HIGH : LOW);
    digitalWrite(StatusIndicator::LED_WARNING, (state & STATE_WARNING) ? HIGH : LOW);
    digitalWrite(StatusIndicator::LED_NOMINAL, (state & STATE_NOMINAL) ? HIGH : LOW);
    digitalWrite(StatusIndicator::LED_INDICAT, (indicator) ? HIGH : LOW);
}

void StatusIndicator::turnOff()
//****************************************************************************************
{
    digitalWrite(StatusIndicator::LED_CRITICAL, LOW);
    digitalWrite(StatusIndicator::LED_WARNING, LOW);
    digitalWrite(StatusIndicator::LED_NOMINAL, LOW);
    digitalWrite(StatusIndicator::LED_INDICAT, LOW);
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
    indicator = enable;
    display();
}

void StatusIndicator::switchIndicator(bool enable, unsigned long duration)
//****************************************************************************************
{
    indicator = enable;
    indicationStart = millis();
    indicationDuration = duration;
    display();
}
void StatusIndicator::switchAllOff()
//****************************************************************************************
{
    state = 0;
    indicator = false;
    display();
}

void StatusIndicator::switchTo(const int state)
//****************************************************************************************
{
    this->state = state;
    display();
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

void StatusIndicator::enableLeds(bool enable)
//****************************************************************************************
{
    enabled = enable;
    display();
}   

void StatusIndicator::switchState(const char *status)
//****************************************************************************************
{
    if (strcmp("critical", status) == 0)
    {
        state = STATE_CRITICAL;
    }
    else if (strcmp("warning", status) == 0)
    {
        state = STATE_WARNING;
    }
    else if (strcmp("nominal", status) == 0)
    {
        state = STATE_NOMINAL;
    }

    display();

    Log.info(MODULE, "State set to %s", status);
}

StatusIndicator statusIndicator;