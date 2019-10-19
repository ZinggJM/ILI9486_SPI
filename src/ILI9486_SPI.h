// created by Jean-Marc Zingg to be a standalone ILI9486_SPI library (instead of the GxCTRL_ILI9486_SPI class for the GxTFT library)
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _ILI9486_SPI_H_
#define _ILI9486_SPI_H_

#include <Arduino.h>
#include <SPI.h>

#include "GFXcanvas16T.h"

#include <Adafruit_GFX.h>

class ILI9486_SPI : public Adafruit_GFX
{
  public:
    ILI9486_SPI(int8_t cs, int8_t dc, int8_t rst);
    // (overridden) virtual methods
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void fillScreen(uint16_t color);
    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(bool i);
    // other public methods
    void init(void);
    void setWindowAddress(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void pushColors(const uint16_t* data, uint16_t n); // fast one
    void setBackLight(bool lit);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
    void drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  private:
    void _startTransaction();
    void _endTransaction();
    void _writeCommand(uint8_t cmd);
    void _writeData(uint8_t data);
    void _writeData16(uint16_t data);
    void _writeData16(uint16_t data, uint32_t n);
    void _writeData16(const uint16_t* data, uint32_t n);
    void _writeCommandTransaction(uint8_t cmd);
    void _writeDataTransaction(uint8_t data);
    void _writeDataTransaction16(uint16_t data);
    void _writeDataTransaction(uint8_t* data, uint32_t n);
  protected:
    uint16_t WIDTH, HEIGHT, _width, _height, _rotation;
  private:
    SPISettings _spi_settings;
    int8_t _cs, _dc, _rst;
    int8_t _bgr;
};

#endif