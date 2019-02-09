#ifndef _WS2812BUFFER_H_
#define _WS2812BUFFER_H_

#include <stdint.h>
//#include "gfxfont.h"
#include "Adafruit_GFX.h"

class WS2812Buffer : public Adafruit_GFX
{
    //Not implemented yet
    WS2812Buffer(const WS2812Buffer&);
    WS2812Buffer& operator=(const WS2812Buffer&);

protected:
    typedef uint16_t xy_t;
    typedef uint32_t colour_t;
    typedef uint8_t opacity_t;
    static const opacity_t max_opacity = -1;
    class pixel
    {
        static uint8_t extractRed(colour_t c) { return c >> 16; }
        static uint8_t extractGreen(colour_t c) { return (c & 0xFF00) >> 8; }
        static uint8_t extractBlue(colour_t c) { return (c & 0xFF); }
        static colour_t toColourType(const uint8_t r, const uint8_t g, const uint8_t b) { return (r<<16) | (g << 8) | b; }
        public:
        colour_t colour;
        opacity_t opacity;

        pixel(colour_t c = 0, opacity_t o = 0) : colour(c), opacity(o) {}
        
        void clear() { colour = opacity = 0;}
        
        pixel operator+(const pixel& rhs) const
        {
            const pixel& lhs = *this;
            const float r1 = extractRed(lhs.colour), g1 = extractGreen(lhs.colour), b1 = extractBlue(lhs.colour);
            const float r2 = extractRed(rhs.colour), g2 = extractGreen(rhs.colour), b2 = extractBlue(rhs.colour);
            const float f = float(lhs.opacity) / float(max_opacity);
            return toColourType(r1*f + r2*(1.-f), g1*f + g2*(1.-f), b1*f + b2*(1.-f));
        }
    };
protected:
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    inline const pixel& getPixel(const xy_t x, const xy_t y) const
    {
        if((x >= width()) | (y >= height()))
            return buffer[0];
        return buffer[x*height() + y];
    }

    inline pixel& getPixel(const xy_t x, const xy_t y)
    {
        if((x >= width()) | (y >= height()))
            return buffer[0];
        return buffer[x*height() + y];
    }


    void setPixelColor(const xy_t x, const xy_t y, const colour_t c, const opacity_t o = 255);
    void setOpacity(const xy_t x, const xy_t y, const opacity_t o = -1);

    bool sizeMatch(const Adafruit_GFX&) const;
protected:
    pixel* const buffer;
public:
    typedef enum 
    {
        OVERWRITE = 1
    } OPERATION_OPTIONS;
public:
    WS2812Buffer(const uint16_t w, const uint16_t h);
    WS2812Buffer(const class Adafruit_NeoMatrix&);
    ~WS2812Buffer();
    void clear();
    colour_t getPixelColor(const xy_t x, const xy_t y) const;
    opacity_t getOpacity(const xy_t x, const xy_t y) const;

    void applyLayer(const WS2812Buffer& rhs, uint8_t options);

    void writeToMatrix(class Adafruit_NeoMatrix&) const;
};

#endif