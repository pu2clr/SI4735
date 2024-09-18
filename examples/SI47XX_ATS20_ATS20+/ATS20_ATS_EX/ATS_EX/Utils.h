#pragma once

const DCfont* LastFont = DEFAULT_FONT;

void oledSetFont(const DCfont* font)
{
    if (font && LastFont != font)
    {
        LastFont = font;
        oled.setFont(font);
    }
}

void oledPrint(const char* text, int offX = -1, int offY = -1, const DCfont* font = LastFont, bool invert = false)
{
    oledSetFont(font);
    if (invert)
        oled.invertOutput(invert);
    if (offX >= 0 && offY >= 0)
        oled.setCursor(offX, offY);
    oled.print(text);
    if (invert)
        oled.invertOutput(false);
}

void oledPrint(uint16_t u, int offX = -1, int offY = -1, const DCfont* font = LastFont, bool invert = false)
{
    oledSetFont(font);
    if (invert)
        oled.invertOutput(invert);
    if (offX >= 0 && offY >= 0)
        oled.setCursor(offX, offY);
    oled.print(u);
    if (invert)
        oled.invertOutput(false);
}

//Faster alternative for convertToChar
void utoa(char* out, uint16_t num)
{
    char* p = out;
    if (num == 0)
        *p++ = '0';
    else
    {
        for (uint16_t base = 10000; base > 0; base /= 10)
        {
            if (num >= base)
            {
                *p++ = '0' + num / base;
                num %= base;
            }
            else if (p != out)
                *p++ = '0';
        }
    }

    *p = '\0';
}

//Better than sprintf which has overwhelmingly large overhead, it helps to reduce binary size
void convertToChar(char* strValue, uint16_t value, uint8_t len, uint8_t dot = 0, uint8_t separator = 0, uint8_t space = ' ')
{
    char d;
    int8_t i;
    for (i = (len - 1); i >= 0; i--)
    {
        d = value % 10;
        value = value / 10;
        strValue[i] = d + 48;
    }
    strValue[len] = '\0';

    if (dot > 0)
    {
        for (int i = len; i >= dot; i--)
        {
            strValue[i + 1] = strValue[i];
        }
        strValue[dot] = separator;
        len = dot;
    }
    i = 0;
    len--;

    while ((i < len) && ('0' == strValue[i]))
    {
        strValue[i++] = space;
    }
}

//Measure integer digit length
int ilen(uint16_t n)
{
    if (n < 10)
        return 1;
    else if (n < 100)
        return 2;
    else if (n < 1000)
        return 3;
    else if (n < 10000)
        return 4;
    else
        return 5;
}

//Split KHz frequency + BFO to KHz and .00 tail
void splitFreq(uint16_t& khz, uint16_t& tail)
{
    int32_t freq = (uint32_t(g_currentFrequency) * 1000) + g_currentBFO;
    khz = freq / 1000;
    tail = abs(freq % 1000) / 10;
}

uint8_t strlen8(const char* str)
{
    uint8_t n = 0;
    while (str[n] != '\0')
        n++;
    return n;
}