#include <stdarg.h>

#include "component/display.h"
#include "logger.h"

#include "Adafruit_SSD1306.h"
#include "appfonts/SFCompactTextS8B8.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

Adafruit_SSD1306 device = Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1);

#define MODULE "DISPLAY"

void Display::setup()
//****************************************************************************************
{
    Wire.begin(5, 4);
    if (!device.begin(SSD1306_SWITCHCAPVCC, 0x3c))
    {
        Log.error(MODULE, "Display could not be started!");
    }
    else
    {
        Log.info(MODULE, "Display initialized");
        device.ssd1306_command(SSD1306_DISPLAYON);
        device.display();
        delay(2000);
        device.clearDisplay();
        
        setDisplayFont(&SFCompactText8pt8b);
        device.setTextWrap(false);
        device.setTextColor(SSD1306_WHITE);
        
        changed = true;
    }
}

void Display::setDisplayFont(const GFXfont *font)
//****************************************************************************************
{
    memcpy_P(&fontData, font, sizeof(GFXfont));
    device.setFont(font);
    maxlines = DISPLAY_HEIGHT / fontData.yAdvance + 1;
}

void Display::loop()
//****************************************************************************************
{
    if (changed)
    {
        Log.debug(MODULE, "Updating display!");
        device.display();
        device.clearDisplay();
        changed = false;
    }
}

void Display::clear()
//****************************************************************************************
{
    changed = true;
}

bool Display::printto(uint8_t line, const char *text, ...)
//****************************************************************************************
{
    va_list args;
    va_start(args, text);
    char buffer[255];
    vsnprintf(buffer, 254, text, args);
    va_end(args);

    if (line > 0 && line <= maxlines)
    {
        device.setCursor(0, (line - 1) * fontData.yAdvance + fontData.yAdvance / 2);
        device.print(buffer);
        Log.info(MODULE, "Putting \"%s\" to line %d", buffer, line);
        changed = true;
        return true;
    }
    else
    {
        return false;
    }
}

Display display;