#include "main.h"
#include "oled.h"

#define SSD1306_WIDTH 128 ///< DEPRECATED: width w/SSD1306_128_64 defined
#define SSD1306_HEIGHT 64 ///< DEPRECATED: height w/SSD1306_128_64 defined

#define COMMAND 0x00
#define DATA 0x40
uint8_t SSD1306_Buffer[SSD1306_WIDTH*SSD1306_HEIGHT/8+8];

