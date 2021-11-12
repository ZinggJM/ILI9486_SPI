// Display Library example for ILI9486 TFT displays using the ILI9486_SPI library
// Requires HW SPI and Adafruit_GFX.
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/ILI9486_SPI

#include <ILI9486_SPI.h> //Hardware-specific library
#include <Fonts/FreeMonoBold9pt7b.h>

// adapt the constructor parameters to your wiring for the appropriate processor conditional,
// or add a new one or adapt the catch all other default

#if (defined(TEENSYDUINO) && (TEENSYDUINO == 147))
ILI9486_SPI tft(/*CS=*/ 10, /*DC=*/ 15, /*RST=*/ 14);
// for my wirings used for e-paper displays:
#elif defined (ESP8266)
ILI9486_SPI tft(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // my D1 mini connection shield for e-paper displays
//ILI9486_SPI tft(/*CS=D8*/ SS, /*DC=D4*/ 2, /*RST=D3*/ 0); // my proto board for RPi display
#elif defined(ESP32)
ILI9486_SPI tft(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16);
#elif defined(_BOARD_GENERIC_STM32F103C_H_)
ILI9486_SPI tft(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2);
#elif defined(__AVR)
ILI9486_SPI tft(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9);
#elif defined(ARDUINO_ARCH_SAM)
ILI9486_SPI tft(/*CS=10*/ SS, /*DC=*/ 6, /*RST=*/ 5); // my proto board
#elif defined(ARDUINO_ARCH_SAMD)
// mapping suggestion for Arduino MKR1000 or MKRZERO
// note: can't use SS on MKR1000: is defined as 24, should be 4
// BUSY -> 5, RST -> 6, DC -> 7, CS-> 4, CLK -> 9, DIN -> 8 // my e-paper connector
ILI9486_SPI tft(/*CS=*/ 4, /*DC=*/ 7, /*RST=*/ 6); // to my proto board
#else
// catch all other default
ILI9486_SPI tft(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9);
#endif

const char HelloWorld[] = "Hello World!";

void setup()
{
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("setup");

  // uncomment for normal SPI mode; default true used for "special" SPI circuit found e.g. on 3.5" RPI HVGA display
  tft.setSpiKludge(false); // false to disable rpi_spi16_mode

  tft.init();
  Serial.println("tft.init() done");

  // the minimum
  tft.setTextColor(0xFFFF);
  tft.fillScreen(0x0000);
  tft.print(HelloWorld);

  delay(1000);

  // some free font, centered
  helloWorld(&FreeMonoBold9pt7b, 0);

  delay(1000);

  Serial.println("setup() done");
}

void loop(void)
{
  for (uint8_t rotation = 0; rotation < 4; rotation++)
  {
    tft.setRotation(rotation);
    helloWorld(&FreeMonoBold9pt7b, rotation);
    delay(10000);
  }
}

void helloWorld(const GFXfont* font, uint8_t rotation)
{
  //Serial.println("helloWorld");
  tft.setRotation(rotation);
  tft.setFont(font);
  tft.setTextColor(0xFFFF);
  int16_t tbx, tby; uint16_t tbw, tbh;
  tft.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x = ((tft.width() - tbw) / 2) - tbx;
  uint16_t y = ((tft.height() - tbh) / 2) - tby;
  tft.fillScreen(0x0000);
  tft.setCursor(x, y);
  tft.print(HelloWorld);
  //Serial.println("helloWorld done");
}
