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

// offset X=34 biasanya untuk LCD 1.47 Waveshare
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
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);

    gfx->begin();
    gfx->setRotation(1);

    gfx->fillScreen(BLACK);

    // border full layar
    gfx->drawRect(0, 0, 320, 172, WHITE);

    gfx->setTextSize(3);
    gfx->setTextColor(GREEN);

    gfx->setCursor(40, 50);
    gfx->println("LAYAR OK");

    gfx->setCursor(40, 100);
    gfx->println("ROTASI 1");

    gfx->drawLine(0, 171, 319, 171, RED);
}

void loop()
{
}