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
    1,      // rotation
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

    // warna background
    gfx->fillScreen(0x0000);

    // frame putih
    gfx->drawRect(0, 0, 172, 320, 0xFFFF);

    // text hijau
    gfx->setTextColor(0x07E0);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("LAYAR OK");

    gfx->setCursor(20, 80);
    gfx->println("ROTASI 1");

    // garis merah bawah
    gfx->drawLine(0, 319, 171, 319, 0xF800);
}

void loop()
{
}