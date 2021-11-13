// Display Library example for ILI9486 TFT displays using the ILI9486_SPI library with library U8g2_for_Adafruit_GFX
// Requires HW SPI and Adafruit_GFX.
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/ILI9486_SPI

// Hello_U8g2_Fonts : show use of U8G2_FOR_ADAFRUIT_GFX with ILI9486_SPI
//
// Library: https://github.com/olikraus/U8g2_for_Adafruit_GFX
// see: https://github.com/olikraus/U8g2_for_Adafruit_GFX/blob/master/README.md
//
// NOTE: you need to SAVE the modified example to a saveable location for UTF-8 characters to work
//       e.g. for Umlauts

#include <ILI9486_SPI.h> //Hardware-specific library
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>

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
const char HelloU8g2[] = "Hello U8g2! ÄÖÜäéöü";

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

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

  u8g2Fonts.begin(tft); // connect u8g2 procedures to Adafruit GFX

  // the minimum
  tft.setTextColor(0xFFFF);
  tft.fillScreen(0x0000);
  tft.print(HelloWorld);

  delay(1000);

  // some free font, centered
  helloWorld(&FreeMonoBold9pt7b, 0);

  delay(1000);

  // some U8g2 font, centered
  helloU8g2(u8g2_font_helvR14_tf, 0);
  delay(2000);
  showFont("u8g2_font_helvR14_tf", u8g2_font_helvR14_tf); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  delay(2000);
  showFont("u8g2_font_profont22_mr", u8g2_font_profont22_mr); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

  delay(1000);

  Serial.println("setup() done");
}

void loop(void)
{
  for (uint8_t rotation = 0; rotation < 4; rotation++)
  {
    tft.setRotation(rotation);
    //helloWorld(&FreeMonoBold9pt7b, rotation);
    helloU8g2(u8g2_font_helvR14_tf, rotation);
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

void helloU8g2(const uint8_t *font, uint8_t rotation)
{
  //Serial.println("helloU8g2");
  tft.setRotation(rotation);
  uint16_t bg = 0xFFFF;
  uint16_t fg = 0x0000;
  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(fg);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(bg);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  int16_t tw = u8g2Fonts.getUTF8Width(HelloU8g2); // text box width
  int16_t ta = u8g2Fonts.getFontAscent(); // positive
  int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
  int16_t th = ta - td; // text box height
  //Serial.print("ascent, descent ("); Serial.print(u8g2Fonts.getFontAscent()); Serial.print(", "); Serial.print(u8g2Fonts.getFontDescent()); Serial.println(")");
  // center bounding box by transposition of origin:
  // y is base line for u8g2Fonts, like for Adafruit_GFX True Type fonts
  uint16_t x = (tft.width() - tw) / 2;
  uint16_t y = (tft.height() - th) / 2 + ta;
  //Serial.print("bounding box    ("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(tw); Serial.print(", "); Serial.print(th); Serial.println(")");
  tft.fillScreen(bg);
  u8g2Fonts.setCursor(x, y); // start writing at this position
  u8g2Fonts.print(HelloU8g2);
  //Serial.println("helloU8g2 done");
}

void showFont(const char name[], const uint8_t *font)
{
  tft.setRotation(0);
  u8g2Fonts.setFontMode(1);                   // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);              // left to right (this is default)
  u8g2Fonts.setForegroundColor(0xFFFF);  // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(0x0000);  // apply Adafruit GFX color
  u8g2Fonts.setFont(font); // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  tft.fillScreen(0x0000);
  u8g2Fonts.setCursor(0, 0);
  u8g2Fonts.println();
  u8g2Fonts.println(name);
  u8g2Fonts.println(" !\"#$%&'()*+,-./");
  u8g2Fonts.println("0123456789:;<=>?");
  u8g2Fonts.println("@ABCDEFGHIJKLMNO");
  u8g2Fonts.println("PQRSTUVWXYZ[\\]^_");
  u8g2Fonts.println("`abcdefghijklmno");
  u8g2Fonts.println("pqrstuvwxyz{|}~ ");
  u8g2Fonts.println("Umlaut ÄÖÜäéöü");
}
