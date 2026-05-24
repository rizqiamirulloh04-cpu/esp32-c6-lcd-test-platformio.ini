#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL 15

Arduino_DataBus *bus = new Arduino_ESP32SPI(
    8,   // DC
    14,  // CS
    7,   // SCK
    6,   // MOSI
    GFX_NOT_DEFINED // MISO
);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus,
    9,    // RST
    0,    // rotation
    true, // IPS
    172,  // width
    320   // height
);

void setup()
{
    Serial.begin(115200);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    delay(200);

    if (!gfx->begin())
    {
        Serial.println("gfx->begin failed");
        while (1);
    }

    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(2);
    gfx->setCursor(20, 80);

    gfx->println("RC Dashboard");

    gfx->setCursor(20, 120);
    gfx->println("ESP32-C6 OK");

    Serial.println("Display OK");
}

void loop()
{
}