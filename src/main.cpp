#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL    15

#define TFT_MOSI  6
#define TFT_SCLK  7
#define TFT_DC    8
#define TFT_RST   9
#define TFT_CS    14

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
    0,      // rotation default
    true,   // IPS
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

    delay(200);

    gfx->begin();

    delay(200);

    // landscape
    gfx->setRotation(1);

    // test warna
    gfx->fillScreen(0x0000);
    delay(500);

    gfx->fillScreen(0xF800);
    delay(500);

    gfx->fillScreen(0x07E0);
    delay(500);

    gfx->fillScreen(0x001F);
    delay(500);

    gfx->fillScreen(0x0000);

    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);

    gfx->setCursor(20, 40);
    gfx->println("ESP32-C6");

    gfx->setCursor(20, 90);
    gfx->println("LCD OK");

    gfx->drawRect(0, 0, 320, 172, 0xFFFF);
}

void loop()
{
}