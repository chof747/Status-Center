#include "controller/device_data_controller.h"
#include "component/button_controls.h"
#include "logger.h"
#include "component/display.h"
#include "config.h"
#include <ESP8266WiFi.h>

#define MODULE "MSG_CTRL"
#define MAX_INDICES 5
#define FIRMWARE_VERSION 1
#define IP_ADDRESS 2
#define MAC_ADDRESS 3
#define WLAN_NAME 4
#define DEVICE_ID 5

DeviceDataController::DeviceDataController(ButtonControls *buttons, ControllerBase *timeout, activation_cb_t cb)
    : ControllerBase(cb), ButtonController(buttons), TimeOutController(CONTROLLER_TIMEOUT), timeoutTarget(timeout),
      label(""), value(""), index(1)
//*********************************************************************************
{
}

void DeviceDataController::activate()
//*********************************************************************************
{
    Log.debug(MODULE, "Device Controller activated");
    attach();
    obtainDeviceData(index);
    restartTimer();
}

void DeviceDataController::loop()
//*********************************************************************************
{
    if (0 != label.compareTo(""))
    {
        display.printtoinv(1, label.c_str());
        display.printwrap(2, value.c_str());
        label = "";
    }

    if (checkTimeForTimeOut())
    {
        gotoController(timeoutTarget);
    }
}

void DeviceDataController::onClick(uint8_t state)
//*********************************************************************************
{
    restartTimer();
    switch (state)
    {
    case ButtonControls::BTN_ACCEPT:
        gotoNext();
        return;

    case ButtonControls::BTN_UP:
        index = (--index == 0) ? MAX_INDICES  : index;
        break;

    case ButtonControls::BTN_DOWN:
        index = (++index > MAX_INDICES) ? 1 : index;
        break;
    }

    obtainDeviceData(index);
}

void DeviceDataController::obtainDeviceData(uint8_t ix)
{
    switch (ix)
    {
    case FIRMWARE_VERSION:
        label = "Firmware";
        value = VERSION;
        break;

    case IP_ADDRESS:
        label = "IP Adresse";
        value = WiFi.localIP().toString();
        break;

    case MAC_ADDRESS:
        label = "MAC Adresse";
        value = WiFi.macAddress();
        break;

    case WLAN_NAME:
        label = "WLAN";
        value = WiFi.SSID();
        break;

    case DEVICE_ID:
        label = "Name";
        value = DEVICE_NAME;
        break;
    }
}