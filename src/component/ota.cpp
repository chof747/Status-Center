#include "component/ota.h"

//#include <ESP8266HTTPUpdateServer.h>
//#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

//ESP8266WebServer server(80);
//ESP8266HTTPUpdateServer updater;

void Ota::setup()
{
    ArduinoOTA.begin();
    //updater.setup(&server);
    //server.begin();
}

void Ota::loop()
{
    ArduinoOTA.handle();
    //server.handleClient();
}

Ota ota;