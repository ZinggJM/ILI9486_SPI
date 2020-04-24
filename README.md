# ILI9486_SPI for Arduino

This library supports ILI9486 SPI TFT to use with Adafruit_GFX

The code is extracted from GxTFT and adapted for single controller use and Adafruit_GFX.

Can be used with the "special" SPI circuit found on RPi and Waveshare TFTs.

### Version 1.0.1
- fixed an error and some warnings
- added some optimizations that were not checked-in yet
- re-tested with ESP8266 Wemos D1 R2 and Arduino Due
#### Version 1.0.0
- replaced RPi SPI kludge handling, better performance
- added method setSpiKludge() 
- rpi_spi16_mode is on per default, switch off by setSpiKludge(false)
#### Version 0.0.2
- added example constructors for Arduino Due to the examples
- tested with Arduino Due
#### Version 0.0.1
- preliminary version
- works with my "3.5" TFT LCD Touch Screen Module 320 x 480 SPI RGB Display For Raspberry Pi B B+"
- https://www.aliexpress.com/item/32661117216.html
- only tested on ESP8266 Wemos D1 R2
