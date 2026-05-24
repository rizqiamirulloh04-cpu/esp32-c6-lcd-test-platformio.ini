#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define TFT_BL    22

#define TFT_MOSI  6
#define TFT_SCLK  7
#define TFT_CS    14
#define TFT_DC    15
#define TFT_RST   21

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
    0,
    34,
    0
);

void setup()
{
    Serial.begin(115200);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    gfx->begin();

    gfx->fillScreen(0x0000);

    // BORDER TEST
    gfx->drawRect(0, 0, 172, 320, 0xFFFF);

    // CENTER LINE
    gfx->drawLine(0, 160, 172, 160, 0xF800);

    // TEXT
    gfx->setTextColor(0x07E0);
    gfx->setTextSize(2);

    gfx->setCursor(20, 40);
    gfx->println("LAYAR OK");

    gfx->setCursor(20, 80);
    gfx->println("ROTASI 1");
}

void loop()
{
}