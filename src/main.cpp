#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_BL    15

#define TFT_MOSI  6
#define TFT_SCLK  7
#define TFT_CS    14
#define TFT_DC    8
#define TFT_RST   9

Adafruit_ST7789 tft = Adafruit_ST7789(
  TFT_CS,
  TFT_DC,
  TFT_RST
);

void setup() {

  Serial.begin(115200);

  pinMode(TFT_BL, OUTPUT);

  // coba HIGH dulu
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init(172, 320);

  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);

  delay(500);

  tft.fillScreen(ST77XX_RED);
  delay(1000);

  tft.fillScreen(ST77XX_GREEN);
  delay(1000);

  tft.fillScreen(ST77XX_BLUE);
  delay(1000);

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  tft.setCursor(20, 40);
  tft.println("HELLO");

  tft.setCursor(20, 70);
  tft.println("ESP32-C6");

  tft.drawRect(5, 5, 310, 160, ST77XX_WHITE);
}

void loop() {

}