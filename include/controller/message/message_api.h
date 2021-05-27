#ifndef MESSAGE_API_H
#define MESSAGE_API_H

#include <String.h>
#include <ArduinoJson.h>

#define EMPTY_MSG { .title = "", .message = "" }


struct message_t {
    String title;
    String message;    
};

class MessageApi
{
    public:

    MessageApi(String endpoint);

    bool accept();
    message_t first();
    message_t next();
    message_t prev();

    private:

    String endpoint;
    StaticJsonDocument<384> message;

    message_t get(const char* url);

};


#endif //MESSAGE_API_H