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
    320
);

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);

    gfx->begin();
    gfx->fillScreen(0x0000);

    gfx->setRotation(1);

    gfx->setCursor(20, 40);
    gfx->setTextSize(2);
    gfx->setTextColor(0xFFFF);

    gfx->println("ESP32-C6");
    gfx->println("LCD TEST");

    gfx->fillRect(20, 100, 80, 40, 0xF800);
    gfx->fillRect(120, 100, 80, 40, 0x07E0);
    gfx->fillRect(220, 100, 80, 40, 0x001F);
}

void loop()
{
}