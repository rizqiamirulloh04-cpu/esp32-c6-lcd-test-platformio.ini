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
    0,
    true,
    172,
    320
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);

    gfx->begin();

    gfx->fillScreen(0x0000);

    gfx->setCursor(20, 80);
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);

    gfx->println("RC Dashboard");

    gfx->drawRect(0, 0, 172, 320, 0xF800);
}

void loop()
{
}