#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "component/component.h"

class Display: public Component
{
    public:        
        void setup();
        void loop();
        
        bool printto(uint8_t line, const char* text, ...);
        uint8_t getMaxLines();
        void setDisplayFont(const GFXfont* font);

    private:
        bool changed;
        GFXfont fontData;
        uint8_t maxlines;
};

extern Display display;

#endif //DISPLAY_H

