#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL   22

#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_DC   15
#define TFT_RST  21
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
    1,      // rotasi landscape
    true,   // IPS
    172,    // width
    320,    // height

    34,     // col offset
    0,      // row offset

    34,     // col offset rotasi
    0,      // row offset rotasi

    34,     // col offset rotasi 2
    0,      // row offset rotasi 2

    0,      // col offset rotasi 3
    34      // row offset rotasi 3
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);

    gfx->begin();

    gfx->fillScreen(BLACK16);

    // Border putih
    gfx->drawRect(0, 0, 320, 172, WHITE16);

    // Teks hijau
    gfx->setTextColor(GREEN16);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("LAYAR OK");

    gfx->setCursor(20, 80);
    gfx->println("ROTASI 1");

    // Garis merah bawah
    gfx->drawLine(0, 171, 319, 171, RED16);
}

void loop()
{
}