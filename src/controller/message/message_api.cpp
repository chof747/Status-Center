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
    this->responseIndex = 0;
}

bool MessageApi::acceptMessage(String responseId)
//*********************************************************************************
{
    return true;
}

bool MessageApi::acceptMessage()
//*********************************************************************************
{
    char url[255];
    sprintf(url, "%s/accept/%s", this->endpoint.c_str(), (const char *)message["id"]);
    return accept(url);
}

message_t MessageApi::firstMessage()
//*********************************************************************************
{
    String url = this->endpoint + "";
    return get(url.c_str());
}

message_t MessageApi::nextMessage()
//*********************************************************************************
{
    if (message.containsKey("id"))
    {
        char url[255];
        sprintf(url, "%s/%s&next", this->endpoint.c_str(), (const char *)message["id"]);
        return get(url);
    }
    else
    {
        return EMPTY_MSG;
    }
}

message_t MessageApi::prevMessage()
//*********************************************************************************
{
    if (message.containsKey("id"))
    {
        char url[255];
        sprintf(url, "%s/%s&prev", this->endpoint.c_str(), (const char *)message["id"]);
        return get(url);
    }
    else
    {
        return EMPTY_MSG;
    }
}

bool MessageApi::hasResponseOptions()
//*********************************************************************************
{
    return (message.containsKey("actions")) &&
           (message["actions"].is<JsonArray>()) &&
           (message["actions"].as<JsonArray>().size() > 0);
}

response_t MessageApi::firstResponse()
//*********************************************************************************
{
    if (hasResponseOptions())
    {
        responseIndex = 0;
        return getCurrentResponse();
    }
    else
    {
        return EMPTY_RESPONSE;
    }

}

response_t MessageApi::nextResponse()
//*********************************************************************************
{
    if (hasResponseOptions())
    {
        responseIndex = (responseIndex >= message["actions"].as<JsonArray>().size() - 1)
        ? 0
        : responseIndex + 1;
        Log.debug(MODULE, "responseIndex = %d, array size = %d", responseIndex, message["actions"].as<JsonArray>().size());
        return getCurrentResponse();
    }
    else
    {
        return EMPTY_RESPONSE;
    }
}

response_t MessageApi::prevResponse()
//*********************************************************************************
{
    if (hasResponseOptions())
    {
        responseIndex = (responseIndex <= 0)
        ? message["actions"].as<JsonArray>().size() - 1
        : responseIndex - 1;
        return getCurrentResponse();
    }
    else
    {
        return EMPTY_RESPONSE;
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
                               ? String((const char *)message["title"])
                               : String("No title");
            result.message = (message.containsKey("message"))
                                 ? String((const char *)message["message"])
                                 : String("");

            
            //serializeJsonPretty(message, Serial);

            Log.debug(MODULE, "Title: %s", result.title.c_str());
            Log.debug(MODULE, "Message: %s", result.message.c_str());
            Log.debug(MODULE, "Message has %s responses", (hasResponseOptions()) ? "some" : "no");
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

response_t MessageApi::getCurrentResponse()
//*********************************************************************************
{
    JsonObject r = JsonObject(message["actions"][responseIndex]);
   
    return
    {
        .id = r.containsKey("id")
                     ? (const char*) r["id"]
                     : "No Id",
        .label = r.containsKey("label") 
                     ? (const char*) r["label"]
                     : NULL
    };
}

bool MessageApi::accept(const char* url)
//*********************************************************************************
{
    http.begin(client, url);
    int rc = http.POST(String(""));

    if (200 == rc)
    {
        return true;
    } 
    else
    {
        Log.error(MODULE, "Received return code %d with message %s", rc, http.getString());
        return false;
    }
}