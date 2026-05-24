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
    1,
    true,
    172,
    320,
    34,
    0
);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->fillScreen(BLACK);

    gfx->setRotation(1);

    gfx->drawRect(0, 0, gfx->width(), gfx->height(), WHITE);

    gfx->setTextColor(GREEN);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("WAVESHARE");

    gfx->setCursor(20, 80);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 120);
    gfx->println("LCD TEST");

    gfx->drawLine(0, gfx->height() - 1,
                  gfx->width() - 1,
                  gfx->height() - 1,
                  RED);
}

void loop()
{
}