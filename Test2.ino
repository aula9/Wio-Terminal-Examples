#include "TFT_eSPI.h"
TFT_eSPI tft;

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // رسم أشكال وألوان مختلفة
  tft.fillRect(0, 0, 50, 50, TFT_RED);
  tft.fillCircle(100, 25, 25, TFT_GREEN);
  tft.fillTriangle(150, 0, 130, 50, 170, 50, TFT_BLUE);
  
  delay(1000);
  
  // كتابة نص بألوان مختلفة
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(10, 100);
  tft.println("Hello Wio Terminal!");
  
  delay(1000);
  tft.fillScreen(TFT_BLACK);
}
