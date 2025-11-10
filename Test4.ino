#include "TFT_eSPI.h"
TFT_eSPI tft;

void setup() {
  tft.begin();
  tft.setRotation(3);
  
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
}

void loop() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  
  // اختبار الزر A
  if (!digitalRead(WIO_KEY_A)) {
    tft.fillRect(0, 0, 100, 80, TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(20, 30);
    tft.println("BUTTON A");
    tone(WIO_BUZZER, 1000, 100);
  }
  
  // اختبار الزر B
  if (!digitalRead(WIO_KEY_B)) {
    tft.fillRect(110, 0, 100, 80, TFT_GREEN);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(130, 30);
    tft.println("BUTTON B");
    tone(WIO_BUZZER, 1500, 100);
  }
  
  // اختبار الزر C
  if (!digitalRead(WIO_KEY_C)) {
    tft.fillRect(220, 0, 100, 80, TFT_BLUE);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(240, 30);
    tft.println("BUTTON C");
    tone(WIO_BUZZER, 2000, 100);
  }
  
  delay(200);
}
