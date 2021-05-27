#include "controller/message/message_api.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "logger.h"

WiFiClient client;
HTTPClient http;

#define MODULE "MSG_API"

MessageApi::MessageApi(String endpoint) : message()
//*********************************************************************************
{
    this->endpoint = endpoint;
}

bool MessageApi::accept()
//*********************************************************************************
{
    return true;
}

message_t MessageApi::first()
//*********************************************************************************
{
    String url = this->endpoint + "";
    return get(url.c_str());
}

message_t MessageApi::next()
//*********************************************************************************
{
    if (message.containsKey("id")) 
    {
        char url[255];
        sprintf(url, "%s/%s&next", this->endpoint.c_str(), (const char*) message["id"]);
        return get(url);
    } 
    else
    {
        return EMPTY_MSG;
    }
}

message_t MessageApi::prev()
//*********************************************************************************
{
    if (message.containsKey("id")) 
    {
        char url[255];
        sprintf(url, "%s/%s&prev", this->endpoint.c_str(), (const char*) message["id"]);
        return get(url);
    } 
    else
    {
        return EMPTY_MSG;
    }
}

message_t MessageApi::get(const char *url)
//*********************************************************************************
{
    message_t result = EMPTY_MSG;

    http.begin(client, url);
    int rc = http.GET();

    if (rc > 0)
    {
        Log.debug(MODULE, "HTTP Result:\n%s\n", http.getString().c_str());
        DeserializationError error = deserializeJson(message, http.getString());
        if (!error)
        {
            result.title = (message.containsKey("title")) 
              ? String((const char*) message["title"]) 
              : String("No title");
            result.message = (message.containsKey("message")) 
              ? String((const char*) message["message"]) 
              : String("");

            Log.debug(MODULE, "Title: %s", result.title.c_str());
            Log.debug(MODULE, "Message: %s", result.message.c_str());
        }
        else
        {
            Log.warn(MODULE, "Wrongly formatted message. Error was: %s", error.f_str());
        }
    }
    else
    {
        Log.error(MODULE, "Got return code %d", rc);
    }

    return result;
}