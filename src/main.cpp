#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL   15

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_DC   8
#define TFT_RST  9
#define TFT_CS   14

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
    1,      // rotation bawaan driver
    true,   // IPS
    172,    // width
    320,    // height
    34,     // col offset 1
    0,      // row offset 1
    34,     // col offset 2
    0       // row offset 2
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    // landscape
    gfx->setRotation(1);

    // background hitam
    gfx->fillScreen(0x0000);

    // border putih
    gfx->drawRect(0, 0, 320, 172, 0xFFFF);

    // text hijau
    gfx->setTextColor(0x07E0);
    gfx->setTextSize(3);

    gfx->setCursor(20, 30);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 70);
    gfx->println("LANDSCAPE");

    gfx->setCursor(20, 110);
    gfx->println("OK");

    // garis bawah merah
    gfx->fillRect(20, 140, 120, 12, 0xF800);

    // garis bawah hijau
    gfx->fillRect(170, 140, 120, 12, 0x07E0);
}

void loop()
{
}