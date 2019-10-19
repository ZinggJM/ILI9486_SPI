
#include "ILI9486_SPI.h"

#if (defined(TEENSYDUINO) && (TEENSYDUINO == 147))
ILI9486_SPI tft(/*CS=*/ 10, /*DC=*/ 15, /*RST=*/ 14);
// for my wirings used for e-paper displays:
#elif defined (ESP8266)
ILI9486_SPI tft(/*CS=D8*/ SS, /*DC=D4*/ 2, /*RST=D3*/ 0); // my proto board
#elif defined(ESP32)
ILI9486_SPI tft(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16);
#elif defined(_BOARD_GENERIC_STM32F103C_H_)
ILI9486_SPI tft(/*CS=4*/ SS, /*DC=*/ 3, /*RST=*/ 2);
#elif defined(__AVR)
ILI9486_SPI tft(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9);
#else
// catch all other default
ILI9486_SPI tft(/*CS=10*/ SS, /*DC=*/ 8, /*RST=*/ 9);
#endif

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  tft.init();
  // my TFT uses BGR, uncomment for RGB panel
  //tft.invertDisplay(false); // set to false for RGB
  Serial.println("init() done");
  tft.fillScreen(BLACK);
  Serial.println("setup done");
  //testTextOnCanvas();
}

void loop()
{
  for (uint8_t r = 0; r < 4; r++)
  {
    //Serial.print("rotation "); Serial.println(r);
    tft.setRotation(r);
    tft.fillScreen(0x0000);
    tft.setCursor(0, 0);
    tft.setTextColor(RED, BLACK);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.println(01234.56789, 2);
    tft.println(0xDEADBEF, HEX);

    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println("Hello");
    tft.println(01234.56789, 2);
    tft.println(0xDEADBEF, HEX);

    tft.setTextColor(BLUE);
    tft.setTextSize(3);
    tft.println("Hello");
    tft.println(01234.56789, 2);
    tft.println(0xDEADBEF, HEX);
    //Serial.print("rotation "); Serial.print(r); Serial.println(" done");

    delay(5000);
  }
  testTextOnCanvas();
}

#if !(defined (ESP8266) || defined (ESP32)) || false

#include "test_canvas.h"

void testTextOnCanvas()
{
  for (uint8_t r = 0; r < 4; r++)
  {
    tft.setRotation(r);
    tft.fillScreen(BLACK);
    uint32_t start = micros();
    uint16_t h = sizeof(test_canvas) / 130 / 2; // reduced size on AVR
    tft.drawRGBBitmap(0, 0, test_canvas, 130, h);
    uint32_t elapsed = micros() - start;
    Serial.print(F("drawRGBBitmap(0, 0, test_canvas, 130, h) took ")); Serial.println(elapsed);
    delay(5000);
  }
}

#else

//GFXcanvas16 canvas(130, 130); // uses heap space

// let the linker complain if not enough ram
GFXcanvas16T<130, 130> canvas; // uses dynamic memory space

void show_canvas_on_screen_timed()
{
  uint32_t start = micros();
  //tft.setWindowAddress(0, 0, canvas.width() - 1, canvas.height() - 1);
  //tft.pushColors(canvas.getBuffer(), canvas.width() * canvas.height()); // 15ms on ESP8266, 13ms on ESP32
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
  uint32_t elapsed = micros() - start;
  Serial.print(F("show_canvas_on_screen    ")); Serial.println(elapsed);
}

void testTextOnCanvas()
{
  canvas.fillScreen(BLACK);
  canvas.setCursor(0, 0);
  canvas.setTextColor(WHITE);  canvas.setTextSize(1);
  canvas.println("Hello World!");
  canvas.setTextColor(YELLOW); canvas.setTextSize(2);
  canvas.println(1234.56);
  canvas.setTextColor(RED);    canvas.setTextSize(3);
  canvas.println(0xDEADBEF, HEX);
  //canvas.println();
  canvas.setTextColor(GREEN);
  canvas.setTextSize(4);
  canvas.println("Groop");
  canvas.setTextSize(2);
  canvas.println("I implore thee,");
  canvas.setTextSize(1);
  canvas.setTextColor(GREEN);
  canvas.println("my foonting turlingdromes.");
  canvas.println("And hooptiously drangle me");
  canvas.println("with crinkly bindlewurdles,");
  canvas.println("Or I will rend thee");
  canvas.println("in the gobberwarts");
  canvas.println("with my blurglecruncheon,");
  canvas.println("see if I don't!");

  static bool first = true;
  //if (first) canvas.print(Serial, "test_canvas", true);
  first = false;

  if ((canvas.width() < tft.width()) || canvas.height() < tft.height())
  {
    tft.fillScreen(BLACK);
  }

  for (uint8_t r = 0; r < 4; r++)
  {
    tft.setRotation(r);
    //tft.fillScreen(BLACK);
    show_canvas_on_screen_timed();
    delay(5000);
  }
}
#endif
