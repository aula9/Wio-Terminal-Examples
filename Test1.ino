#include "TFT_eSPI.h"

TFT_eSPI tft;

void setup() {
  // تهيئة الشاشة
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);

  // تهيئة المنافذ
  pinMode(WIO_LIGHT, INPUT);
  pinMode(WIO_BUZZER, OUTPUT);
  
  // الأزرار العلوية
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  
  // عصا التوجيه 5-طريق
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  // بدء الاتصال التسلسلي
  Serial.begin(115200);
  
  tft.drawString("Wio Terminal Test", 20, 10);
  delay(2000);
}

void loop() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);

  // اختبار الأزرار العلوية
  tft.println("Top Buttons Test:");
  tft.print("A: "); tft.println(!digitalRead(WIO_KEY_A)); // inverted because of PULLUP
  tft.print("B: "); tft.println(!digitalRead(WIO_KEY_B));
  tft.print("C: "); tft.println(!digitalRead(WIO_KEY_C));
  
  // اختبار عصا التوجيه 5-طريق
  tft.println("\n5-Way Switch:");
  tft.print("Up: "); tft.println(!digitalRead(WIO_5S_UP));
  tft.print("Down: "); tft.println(!digitalRead(WIO_5S_DOWN));
  tft.print("Left: "); tft.println(!digitalRead(WIO_5S_LEFT));
  tft.print("Right: "); tft.println(!digitalRead(WIO_5S_RIGHT));
  tft.print("Press: "); tft.println(!digitalRead(WIO_5S_PRESS));

  // اختبار مستشعر الضوء
  int light = analogRead(WIO_LIGHT);
  tft.print("\nLight: ");
  tft.println(light);

  // اختبار السماعة (سيصدر صوت كل 5 ثواني)
  static unsigned long lastBeep = 0;
  if (millis() - lastBeep > 5000) {
    tone(WIO_BUZZER, 1000, 100);
    lastBeep = millis();
  }
  
  delay(500);
  
  // إرسال البيانات عبر Serial للمراقبة
  Serial.print("Light Sensor: ");
  Serial.println(light);
}
