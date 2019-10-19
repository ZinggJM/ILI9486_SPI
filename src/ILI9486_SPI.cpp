// created by Jean-Marc Zingg to be a standalone ILI9486_SPI library (instead of the GxCTRL_ILI9486_SPI class for the GxTFT library)
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include <SPI.h>
#include "ILI9486_SPI.h"

#define SPI_SPEED 27000000
//#define SPI_SPEED 4000000

#define ILI9486_SPI_CASET 0x2A
#define ILI9486_SPI_PASET 0x2B
#define ILI9486_SPI_RAMWR 0x2C
#define ILI9486_SPI_RAMRD   0x2E
#define ILI9486_SPI_MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

ILI9486_SPI::ILI9486_SPI(int8_t cs, int8_t dc, int8_t rst) : Adafruit_GFX(320, 480), _spi_settings(SPI_SPEED, MSBFIRST, SPI_MODE0)
{
  _cs = cs;
  _dc = dc;
  _rst = rst;
  _bgr = MADCTL_BGR;
  digitalWrite(_cs, HIGH);
  digitalWrite(_dc, HIGH);
  pinMode(_cs, OUTPUT);
  pinMode(_dc, OUTPUT);

  if (rst >= 0)
  {
    digitalWrite(rst, HIGH);
    pinMode(rst, OUTPUT);
  }
  _rotation = 0;
  WIDTH = 320;
  HEIGHT = 480;
  _width = WIDTH;
  _height = HEIGHT;
}

// *** (overridden) virtual methods ***

void ILI9486_SPI::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
  {
    return;
  }
  setWindowAddress(x, y, x, y);
  _writeDataTransaction16(color);
}

void ILI9486_SPI::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  //  if ((x < 0) || (y < 0) || (w < 1) || (h < 1) || (x + w > _width) || (y + h > _height))
  //  {
  //    Serial.print("fillRect("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(") oops? "); delay(1);
  //  }
  // a correct clipping is the goal. try to achieve this
  if (x < 0) w += x, x = 0;
  if (y < 0) h += y, y = 0;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if ((w < 1) || (h < 1)) return;
  setWindowAddress(x, y, x + w - 1, y + h - 1);
  digitalWrite(_dc, HIGH);
  _startTransaction();
  _writeData16(color, uint32_t(w) * uint32_t(h));
  _endTransaction();
}

void ILI9486_SPI::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  fillRect(x, y, 1, h, color);
}

void ILI9486_SPI::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  fillRect(x, y, w, 1, color);
}

void ILI9486_SPI::fillScreen(uint16_t color)
{
  fillRect(0, 0, WIDTH, HEIGHT, color);
}

void ILI9486_SPI::setRotation(uint8_t r)
{
  Adafruit_GFX::setRotation(r);
  rotation = r & 3;
  _width = (rotation & 1) ? HEIGHT : WIDTH;
  _height = (rotation & 1) ? WIDTH : HEIGHT;
  // this controller needs separate transactions; reason _dc before _cs
  // _dc Data/Command needs be set before activating CS
  _writeCommandTransaction(ILI9486_SPI_MADCTL);
  switch (r & 3)
  {
    case 0:
      _writeDataTransaction(MADCTL_MX | _bgr);
      break;
    case 1:
      _writeDataTransaction(MADCTL_MV | _bgr);
      break;
    case 2:
      _writeDataTransaction(MADCTL_MY | _bgr);
      break;
    case 3:
      _writeDataTransaction(MADCTL_MX | MADCTL_MY | MADCTL_MV | _bgr);
      break;
  }
}

void ILI9486_SPI::invertDisplay(bool i)
{
  _bgr = i ? MADCTL_BGR : 0x00;
  setRotation(rotation);
}

// *** other public methods ***

void ILI9486_SPI::init(void)
{
  digitalWrite(_cs, HIGH);
  SPI.begin();
  //SPI.beginTransaction( { SPI_SPEED, MSBFIRST, SPI_MODE0 } );
  if (_rst >= 0)
  {
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
  }
  delay(200);
  setBackLight(true);
  delay(200);
  // this controller needs separate transactions; reason _dc before _cs
  // _dc Data/Command needs be set before activating CS
  _writeCommandTransaction(0x3A);
  _writeDataTransaction(0x55);  // use 16 bits per pixel color
  _writeCommandTransaction(0x36);
  _writeDataTransaction(0x48);  // MX, BGR == rotation 0
  // PGAMCTRL(Positive Gamma Control)
  _writeCommandTransaction(0xE0);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x1F);
  _writeDataTransaction(0x1C);
  _writeDataTransaction(0x0C);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x08);
  _writeDataTransaction(0x48);
  _writeDataTransaction(0x98);
  _writeDataTransaction(0x37);
  _writeDataTransaction(0x0A);
  _writeDataTransaction(0x13);
  _writeDataTransaction(0x04);
  _writeDataTransaction(0x11);
  _writeDataTransaction(0x0D);
  _writeDataTransaction(0x00);
  // NGAMCTRL(Negative Gamma Control)
  _writeCommandTransaction(0xE1);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x32);
  _writeDataTransaction(0x2E);
  _writeDataTransaction(0x0B);
  _writeDataTransaction(0x0D);
  _writeDataTransaction(0x05);
  _writeDataTransaction(0x47);
  _writeDataTransaction(0x75);
  _writeDataTransaction(0x37);
  _writeDataTransaction(0x06);
  _writeDataTransaction(0x10);
  _writeDataTransaction(0x03);
  _writeDataTransaction(0x24);
  _writeDataTransaction(0x20);
  _writeDataTransaction(0x00);
  // Digital Gamma Control 1
  _writeCommandTransaction(0xE2);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x32);
  _writeDataTransaction(0x2E);
  _writeDataTransaction(0x0B);
  _writeDataTransaction(0x0D);
  _writeDataTransaction(0x05);
  _writeDataTransaction(0x47);
  _writeDataTransaction(0x75);
  _writeDataTransaction(0x37);
  _writeDataTransaction(0x06);
  _writeDataTransaction(0x10);
  _writeDataTransaction(0x03);
  _writeDataTransaction(0x24);
  _writeDataTransaction(0x20);
  _writeDataTransaction(0x00);
  _writeCommandTransaction(0x11);  // Sleep OUT
  delay(150);   // wait some time
  _writeCommandTransaction(0x29);  // Display ON
}

#if 0 // doesn't work
void ILI9486_SPI::setWindowAddress(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  // this controller needs separate transactions; reason _dc before _cs
  // _dc Data/Command needs be set before activating CS
  uint8_t data[4];
  _writeCommandTransaction(ILI9486_SPI_CASET);  // Column addr set
  data[0] = (x1 >> 8);
  data[1] = (x1 & 0xFF); // XSTART
  data[2] = (x2 >> 8);
  data[3] = (x2 & 0xFF); // XEND
  _writeDataTransaction(data, 4);
  _writeCommandTransaction(ILI9486_SPI_PASET);  // Row addr set
  data[0] = (y1 >> 8);
  data[1] = (y1);        // YSTART
  data[2] = (y2 >> 8);
  data[3] = (y2);        // YEND
  _writeDataTransaction(data, 4);
  _writeCommandTransaction(ILI9486_SPI_RAMWR);  // write to RAM
}
#else
void ILI9486_SPI::setWindowAddress(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  // this controller needs separate transactions; reason _dc before _cs
  // _dc Data/Command needs be set before activating CS
  _writeCommandTransaction(ILI9486_SPI_CASET);  // Column addr set
  _writeDataTransaction(x1 >> 8);
  _writeDataTransaction(x1 & 0xFF); // XSTART
  _writeDataTransaction(x2 >> 8);
  _writeDataTransaction(x2 & 0xFF); // XEND
  _writeCommandTransaction(ILI9486_SPI_PASET);  // Row addr set
  _writeDataTransaction(y1 >> 8);
  _writeDataTransaction(y1);        // YSTART
  _writeDataTransaction(y2 >> 8);
  _writeDataTransaction(y2);        // YEND
  _writeCommandTransaction(ILI9486_SPI_RAMWR);  // write to RAM
}
#endif
void ILI9486_SPI::pushColors(const uint16_t* data, uint16_t n)
{
  _startTransaction();
  _writeData16(data, n);
  _endTransaction();
}

void ILI9486_SPI::setBackLight(bool lit)
{
  _writeCommandTransaction(lit ? 0x29 : 0x28);  // Display ON / Display OFF
}

uint16_t ILI9486_SPI::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// code extract taken from Adafruit_SPITFT::drawRGBBitmap https://github.com/adafruit/Adafruit-GFX-Library
void ILI9486_SPI::drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h)
{
  if (!pcolors) return; // canvas might be empty
  int16_t x2, y2; // Lower-right coord
  if (( x             >= _width ) ||     // Off-edge right
      ( y             >= _height) ||      // " top
      ((x2 = (x + w - 1)) <  0      ) ||  // " left
      ((y2 = (y + h - 1)) <  0)     ) return; // " bottom

  int16_t bx1 = 0, by1 = 0; // Clipped top-left within bitmap
  int16_t saveW = w;        // Save original bitmap width value
  if (x < 0) // Clip left
  {
    w  +=  x;
    bx1 = -x;
    x   =  0;
  }
  if (y < 0) // Clip top
  {
    h  +=  y;
    by1 = -y;
    y   =  0;
  }
  if (x2 >= _width ) w = _width  - x; // Clip right
  if (y2 >= _height) h = _height - y; // Clip bottom

  pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
  setWindowAddress(x, y, x + w - 1, y + h - 1); // Clipped area
  _startTransaction();
  while (h--) // For each (clipped) scanline...
  {
    _writeData16(pcolors, w); // Push one (clipped) row
    pcolors += saveW; // Advance pointer by one full (unclipped) line
  }
  _endTransaction();
}

// code extract taken from Adafruit_SPITFT::drawRGBBitmap https://github.com/adafruit/Adafruit-GFX-Library
void ILI9486_SPI::drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
  const uint16_t* pcolors = bitmap;
  int16_t x2, y2; // Lower-right coord
  if (( x             >= _width ) ||     // Off-edge right
      ( y             >= _height) ||      // " top
      ((x2 = (x + w - 1)) <  0      ) ||  // " left
      ((y2 = (y + h - 1)) <  0)     ) return; // " bottom

  int16_t bx1 = 0, by1 = 0; // Clipped top-left within bitmap
  int16_t saveW = w;        // Save original bitmap width value
  if (x < 0) // Clip left
  {
    w  +=  x;
    bx1 = -x;
    x   =  0;
  }
  if (y < 0) // Clip top
  {
    h  +=  y;
    by1 = -y;
    y   =  0;
  }
  if (x2 >= _width ) w = _width  - x; // Clip right
  if (y2 >= _height) h = _height - y; // Clip bottom

  pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
  setWindowAddress(x, y, x + w - 1, y + h - 1); // Clipped area
  _startTransaction();
  while (h--) // For each (clipped) scanline...
  {
    for (int16_t i = 0; i < w; i++) // Push one (clipped) row
    {
      _writeData16(pgm_read_word(pcolors + i));
    }
    pcolors += saveW; // Advance pointer by one full (unclipped) line
  }
  _endTransaction();
}

// *** private methods

void ILI9486_SPI::_startTransaction()
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
}

void ILI9486_SPI::_endTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void ILI9486_SPI::_writeCommand(uint8_t cmd)
{
  digitalWrite(_dc, LOW);
  SPI.transfer(cmd);
  digitalWrite(_dc, HIGH);
}

void ILI9486_SPI::_writeData(uint8_t data)
{
  digitalWrite(_dc, HIGH);
  SPI.transfer(data);
}

void ILI9486_SPI::_writeData16(uint16_t data)
{
#if (defined (ESP8266) || defined(ESP32))
  SPI.write16(data);
#else
  SPI.transfer(data >> 8);
  SPI.transfer(data);
#endif
}

void ILI9486_SPI::_writeData16(uint16_t data, uint32_t n)
{
#if (defined (ESP8266) || defined(ESP32)) && true // fastest
  uint16_t swapped = ((data << 8) | (data >> 8));
  SPI.writePattern((uint8_t*)&swapped, 2, n);
#elif (defined (ESP8266) || defined(ESP32))
  uint16_t swapped = ((data << 8) | (data >> 8));
  while (n-- > 0)
  {
    SPI.write16(swapped);
  }
#else // wdt on ESP8266
  while (n-- > 0)
  {
    SPI.transfer(data >> 8);
    SPI.transfer(data);
  }
#endif
}

void ILI9486_SPI::_writeData16(const uint16_t* data, uint32_t n)
{
#if (defined (ESP8266) || defined(ESP32) || (TEENSYDUINO == 147)) && true // fastest
  static const uint16_t swap_buffer_size = 64; // optimal for ESP8266 SPI
  static const uint32_t max_chunk = swap_buffer_size / 2; // uint16_t's
  uint8_t swap_buffer[swap_buffer_size];
  const uint8_t* p1 = reinterpret_cast<const uint8_t*> (data);
  const uint8_t* p2 = p1 + 1;
  while (n > 0)
  {
    uint32_t chunk = min(max_chunk, n);
    n -= chunk;
    uint8_t* p3 = swap_buffer;
    uint8_t* p4 = p3 + 1;
    uint16_t ncopy = chunk;
    while (ncopy-- > 0)
    {
      *p3 = *p2; p3 += 2; p2 += 2;
      *p4 = *p1; p4 += 2; p1 += 2;
    }
#if (defined (ESP8266) || defined(ESP32))
    SPI.transferBytes(swap_buffer, 0, 2 * chunk);
#else
    SPI.transfer(swap_buffer, 0, 2 * chunk);
#endif
  }
#else
  while (n-- > 0)
  {
    uint16_t color = (*data++);
#if (defined (ESP8266) || defined(ESP32)) && false // faster
    SPI.write16(color);
#else
    SPI.transfer(color >> 8);
    SPI.transfer(color);
#endif
  }
#endif
}

void ILI9486_SPI::_writeCommandTransaction(uint8_t cmd)
{
  SPI.beginTransaction(_spi_settings);
  digitalWrite(_dc, LOW); //this controller needs _dc before _cs
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(cmd);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  digitalWrite(_dc, HIGH);
  SPI.endTransaction();
}

void ILI9486_SPI::_writeDataTransaction(uint8_t data)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(data);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}

void ILI9486_SPI::_writeDataTransaction16(uint16_t data)
{
  _startTransaction();
  _writeData16(data);
  _endTransaction();
}

void ILI9486_SPI::_writeDataTransaction(uint8_t* data, uint32_t n)
{
  SPI.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  SPI.transfer(data, n);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  SPI.endTransaction();
}
