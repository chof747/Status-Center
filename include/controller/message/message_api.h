#ifndef MESSAGE_API_H
#define MESSAGE_API_H

#include <String.h>
#include <ArduinoJson.h>

#define EMPTY_MSG { .title = "", .message = "" }
#define EMPTY_RESPONSE { .id = NULL, .label = NULL }

struct response_t {
    const char* id;
    const char* label;
};

struct message_t {
    String title;
    String message;    
};

class MessageApi
{
    public:

    MessageApi(String endpoint);

    //message functions

    bool acceptMessage();
    bool acceptMessage(String responseId);
    message_t firstMessage();
    message_t nextMessage();
    message_t prevMessage();

    bool hasResponseOptions();
    response_t firstResponse();
    response_t nextResponse();
    response_t prevResponse();


    private:

    String endpoint;
    StaticJsonDocument<384> message;
    int responseIndex;

    message_t get(const char* url);
    response_t getCurrentResponse();

};


#endif //MESSAGE_API_H