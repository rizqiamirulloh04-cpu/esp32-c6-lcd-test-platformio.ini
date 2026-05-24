#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_CS   14
#define TFT_DC   15
#define TFT_RST  21
#define TFT_BL   22

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    TFT_DC,
    TFT_CS,
    TFT_SCLK,
    TFT_MOSI,
    GFX_NOT_DEFINED
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    TFT_RST,
    0,
    true,
    172,
    320,
    34,
    0,
    34,
    0
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->fillScreen(0xF800);
    delay(1000);

    gfx->fillScreen(0x07E0);
    delay(1000);

    gfx->fillScreen(0x001F);
    delay(1000);

    gfx->fillScreen(0x0000);

    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("WAVESHARE");

    gfx->setCursor(20, 80);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 120);
    gfx->println("LCD OK");
}

void loop()
{
}