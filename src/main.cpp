#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 15

// SPI LCD
#define TFT_CS    14
#define TFT_DC    8
#define TFT_RST   9
#define TFT_SCLK  7
#define TFT_MOSI  6

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
    Serial.begin(115200);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    delay(200);

    gfx->begin(40000000);

    gfx->setRotation(1);

    gfx->fillScreen(0x0000);

    delay(500);

    gfx->fillScreen(0xF800);
    delay(1000);

    gfx->fillScreen(0x07E0);
    delay(1000);

    gfx->fillScreen(0x001F);
    delay(1000);

    gfx->fillScreen(0x0000);

    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);
    gfx->setCursor(20, 40);
    gfx->println("WAVESHARE");

    gfx->setCursor(20, 90);
    gfx->println("ESP32-C6");

    gfx->drawRect(10, 10, 300, 150, 0xFFFF);
}

void loop()
{
}