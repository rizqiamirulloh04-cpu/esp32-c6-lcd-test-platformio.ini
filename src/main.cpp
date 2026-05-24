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
    0,      // col offset 2
    0       // row offset 2
);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    // background hitam
    gfx->fillScreen(0x0000);

    // border putih full layar
    gfx->drawRect(0, 0, 172, 320, 0xFFFF);

    // garis tengah biru
    gfx->drawLine(0, 160, 171, 160, 0x001F);

    // garis bawah merah
    gfx->drawLine(0, 319, 171, 319, 0xF800);

    // text hijau
    gfx->setTextColor(0x07E0);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("LAYAR OK");

    gfx->setCursor(20, 80);
    gfx->println("OFFSET FIX");

    gfx->setCursor(20, 120);
    gfx->println("ROTASI 1");
}

void loop()
{
}