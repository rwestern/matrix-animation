#include "WS2812Buffer.h"
#include "Adafruit_NeoMatrix.h"

uint32_t expandColor(uint16_t color); // Purloined from Adafruit_NeoMatrix.cpp

WS2812Buffer::WS2812Buffer(const uint16_t w, const uint16_t h)
: Adafruit_GFX(w, h), buffer(new pixel[h*w])
{
    clear();
}

WS2812Buffer::~WS2812Buffer()
{
    delete[] buffer;
}

void WS2812Buffer::clear()
{
    for(pixel*p=buffer;p!=buffer+width()*height();++p)
        p->clear();
}

void WS2812Buffer::drawPixel(int16_t x, int16_t y, uint16_t c)
{
    pixel& p = getPixel(x, y);
    p.colour = c;
    p.opacity = 255;
}

WS2812Buffer::colour_t WS2812Buffer::getPixelColor(const xy_t x, const xy_t y) const
{
    return getPixel(x, y).colour;
}

void WS2812Buffer::setPixelColor(const xy_t x, const xy_t y, const colour_t c, const opacity_t o)
{
    pixel& p = getPixel(x, y);
    p.colour = expandColor(c);
    p.opacity = o;
}

void WS2812Buffer::setOpacity(const xy_t x, const xy_t y, const opacity_t o)
{
    pixel& p = getPixel(x, y);
    p.opacity = o;
}

WS2812Buffer::opacity_t WS2812Buffer::getOpacity(const xy_t x, const xy_t y) const
{
    return getPixel(x, y).opacity;
}

void WS2812Buffer::applyLayer(const WS2812Buffer& rhs, uint8_t options)
{
    if(!sizeMatch(rhs))
        return;
    if(options & OVERWRITE)
    {
        for(uint16_t n=0; n<height()*width();++n)
        {
            if(rhs.buffer[n].opacity != 0)
            {
                this->buffer[n] = this->buffer[n] + rhs.buffer[n];
            }
            else
            {
                this->buffer[n] = rhs.buffer[n];
            }
        }
    }
}

void WS2812Buffer::writeToMatrix(Adafruit_NeoMatrix& matrix) const
{
    if( !sizeMatch(matrix))
        return;
    for(int16_t x=0;x<width();++x)
    {
        for(int16_t y=0;y<height();++y)
        {
            //if(getOpacity(x, y) != 0)
            {
                matrix.writePixel(x, y, getPixelColor(x, y));
            }
        }
    }
}

bool WS2812Buffer::sizeMatch(const Adafruit_GFX& rhs) const
{
    return ((rhs.height() == this->height()) & (rhs.width() == this->width()));
}
