#include "TFT_eSPI.h"
TFT_eSPI tft;

void setup() {
  tft.begin();
  tft.setRotation(3);
  Serial.begin(115200);
}

void loop() {
  int lightValue = analogRead(WIO_LIGHT);
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 50);
  tft.print("Light: ");
  tft.println(lightValue);
  
  // رسم شريط بياني للضوء
  int barWidth = map(lightValue, 0, 1023, 0, 200);
  tft.fillRect(20, 120, barWidth, 30, TFT_CYAN);
  
  Serial.print("Light Level: ");
  Serial.println(lightValue);
  
  delay(500);
}
