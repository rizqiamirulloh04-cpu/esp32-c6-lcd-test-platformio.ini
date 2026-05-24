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

    pinMode(TFT_BL, OUTPUT);

    // coba HIGH dulu
    digitalWrite(TFT_BL, HIGH);

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

    gfx->setCursor(20, 40);
    gfx->setTextColor(0xFFFF);
    gfx->setTextSize(3);

    gfx->println("RC");
    gfx->println("DASHBOARD");
}

void loop()
{
}