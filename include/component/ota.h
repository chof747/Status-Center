#ifndef OTA_H
#define OTA_H

#include "component.h"

class Ota: public Component
{
    public:

        void setup();
        void loop();

    private:

};

extern Ota ota;

#endif //STATUS_INDICATOR_H