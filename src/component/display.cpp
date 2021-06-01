#include <stdarg.h>
#include <string.h>

#include "component/display.h"
#include "logger.h"

#include "Adafruit_SSD1306.h"
#include "appfonts/SFCompactTextS8B8.h"
#include "Adafruit_GFX.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define GROUND_LINE (line - 1) * fontData.yAdvance + fontData.yAdvance / 2 + 1

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
    spaceWidth = calcTextWidth("  ", 0); 
    Log.debug(MODULE, "spacewidth = %d", spaceWidth);
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
        device.setCursor(0, GROUND_LINE);
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

bool Display::printtoinv(uint8_t line, const char *text)
//****************************************************************************************
{
    bool result = false;
    device.fillRect(0, (line - 1) * fontData.yAdvance, device.width(), line * fontData.yAdvance * 3 / 4 + 1, SSD1306_WHITE);
    device.setTextColor(SSD1306_BLACK);
    result = printto(line, text);
    device.setTextColor(SSD1306_WHITE);

    return result;
}

uint16_t Display::printwrap(uint8_t line, const char *text)
//****************************************************************************************
{
    char buf[strlen(text)+1];
    strcpy(buf, text);
    char *ptr = strtok(buf, " ");
    uint16_t xpos = 0;
    uint16_t cur = 0;

    while (ptr != NULL)
    {
        //increasing the cursor by one to indicate the delimiter between words
        //do this here because if the next word is already going beyond the display the
        //space should be already taken into account
        cur++;


        uint16_t w = calcTextWidth(ptr, xpos);
        if ((xpos + w) > device.width())
        {
            ++line;
            if (line > maxlines)
                break;
            xpos = 0;
        }

        device.setCursor(xpos, GROUND_LINE);
        device.print(ptr);
        xpos += (w + spaceWidth);

        cur += strlen(ptr);
        ptr = strtok(NULL, " ");
    }

    //return the cursor - 1 as we have initially added a space also after the last word
    return --cur;
}

uint16_t Display::calcTextWidth(const char *text, uint16_t xpos)
//****************************************************************************************
{
    int16_t x1, y1;
    uint16_t w, h;
    
    device.getTextBounds(text, xpos, 0, &x1, &y1, &w, &h);
    return w * 1;
}

Display display;