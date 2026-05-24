#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_MOSI  6
#define TFT_SCLK  7
#define TFT_CS    14
#define TFT_DC    15
#define TFT_RST   21
#define TFT_BL    22

SPIClass spi = SPIClass(FSPI);

Adafruit_ST7789 tft = Adafruit_ST7789(&spi, TFT_CS, TFT_DC, TFT_RST);

void setup() {

  Serial.begin(115200);

  // Backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // SPI LCD
  spi.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Init LCD
  tft.init(172, 320);

  tft.setRotation(1);

  // Offset penting untuk 1.47"
  tft.setAddrWindow(34, 0, 172, 320);

  // Test warna
  tft.fillScreen(ST77XX_RED);
  delay(1000);

  tft.fillScreen(ST77XX_GREEN);
  delay(1000);

  tft.fillScreen(ST77XX_BLUE);
  delay(1000);

  tft.fillScreen(ST77XX_BLACK);

  // Text
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  tft.setCursor(20, 40);
  tft.println("RC DASHBOARD");

  tft.setCursor(20, 80);
  tft.println("ESP32-C6");

  tft.drawRect(5, 5, 310, 160, ST77XX_WHITE);
}

void loop() {
}