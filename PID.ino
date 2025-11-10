#include "TFT_eSPI.h"
TFT_eSPI tft;

// PID variables
double Kp = 3.0;
double Ki = 0.2; 
double Kd = 0.8;

double setpoint = 500;
double error = 0;
double lastError = 0;
double integral = 0;
double derivative = 0;
double output = 0;

unsigned long lastTime = 0;
unsigned long sampleTime = 100;

int graphData[60];
int dataIndex = 0;

const int EXTERNAL_LED = D1;

void drawInterface();
void updateDisplay(int lightValue, int output);
void updateGraph(int lightValue);
void drawGraph();
void checkButtons();
void drawLightBar(int lightValue);
void clearDisplayArea(int x, int y, int w, int h);

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  
  pinMode(WIO_LIGHT, INPUT);
  pinMode(WIO_BUZZER, OUTPUT);
  pinMode(EXTERNAL_LED, OUTPUT);
  
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  
  Serial.begin(115200);
  
  drawInterface();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastTime >= sampleTime) {
    int lightValue = analogRead(WIO_LIGHT);
    
    // PID calculation with anti-windup
    error = setpoint - lightValue;
    
    if (!(output >= 250 && error > 0) && !(output <= 5 && error < 0)) {
      integral += error;
    }
    
    integral = constrain(integral, -1000, 1000);
    derivative = error - lastError;
    
    output = Kp * error + Ki * integral + Kd * derivative;
    output = constrain(output, 0, 255);
    
    updateGraph(lightValue);
    updateDisplay(lightValue, output);
    
    analogWrite(EXTERNAL_LED, output);
    
    lastError = error;
    lastTime = currentTime;
    
    Serial.print("Setpoint:");
    Serial.print(setpoint);
    Serial.print(", Light:");
    Serial.print(lightValue);
    Serial.print(", Output:");
    Serial.print(output);
    Serial.print(", Error:");
    Serial.println(error);
  }
  
  checkButtons();
  delay(10);
}

void drawInterface() {
  tft.fillScreen(TFT_BLACK);
  
  // Title at top
  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(70, 5);
  tft.println("BRIGHTNESS CONTROL");
  
  // Separator line
  tft.drawFastHLine(0, 30, 320, TFT_DARKGREY);
  
  // Left column - PID parameters
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 35);
  tft.println("PID PARAMETERS:");
  tft.setCursor(10, 50);
  tft.print("Kp: "); tft.println(Kp, 1);
  tft.setCursor(10, 65);
  tft.print("Ki: "); tft.println(Ki, 2);
  tft.setCursor(10, 80);
  tft.print("Kd: "); tft.println(Kd, 1);
  
  // Right column - Real-time values (headers only)
  tft.setCursor(180, 35);
  tft.println("CURRENT VALUES:");
  
  // Button instructions at bottom of text area
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(10, 100);
  tft.println("A: -Target  B: +Target  C: Tune PID");
  
  // Draw initial static elements for light bar and graph
  drawLightBar(0);
  drawGraph();
}

void updateDisplay(int lightValue, int output) {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  
  // Update real-time values (right column)
  clearDisplayArea(180, 50, 130, 45);
  
  tft.setTextSize(1);
  tft.setCursor(180, 50);
  tft.print("Target: "); tft.println(setpoint);
  
  tft.setCursor(180, 65);
  tft.print("Light:  "); tft.println(lightValue);
  
  tft.setCursor(180, 80);
  tft.print("Output: "); tft.println(output);
  
  // Update light bar and graph (below all text)
  drawLightBar(lightValue);
  drawGraph();
}

void clearDisplayArea(int x, int y, int w, int h) {
  tft.fillRect(x, y, w, h, TFT_BLACK);
}

void drawLightBar(int lightValue) {
  int barWidth = map(lightValue, 0, 1023, 0, 250);
  int barHeight = 15;
  int barX = 35;
  int barY = 120;  // Moved below all text
  
  // Clear bar area
  tft.fillRect(barX, barY, 255, barHeight + 5, TFT_BLACK);
  
  // Bar background
  tft.fillRect(barX, barY, 250, barHeight, TFT_DARKGREY);
  
  // Bar foreground with color coding
  uint16_t barColor;
  if (lightValue < 300) barColor = TFT_RED;
  else if (lightValue < 700) barColor = TFT_YELLOW;
  else barColor = TFT_GREEN;
  
  tft.fillRect(barX, barY, barWidth, barHeight, barColor);
  tft.drawRect(barX, barY, 250, barHeight, TFT_WHITE);
  
  // Bar label
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, barY + 3);
  tft.print("Light Level:");
  tft.setCursor(barX + 255, barY + 3);
  tft.print(lightValue);
}

void updateGraph(int lightValue) {
  graphData[dataIndex] = lightValue;
  dataIndex = (dataIndex + 1) % 60;
}

void drawGraph() {
  int graphHeight = 70;
  int graphWidth = 280;
  int graphX = 20;
  int graphY = 145;  // Moved below light bar
  
  // Clear graph area
  tft.fillRect(graphX, graphY, graphWidth + 5, graphHeight + 5, TFT_BLACK);
  
  // Graph border
  tft.drawRect(graphX, graphY, graphWidth, graphHeight, TFT_DARKGREY);
  
  // Setpoint line
  int setpointY = graphY + graphHeight - map(setpoint, 0, 1023, 0, graphHeight);
  tft.drawLine(graphX, setpointY, graphX + graphWidth, setpointY, TFT_GREEN);
  
  // Graph data lines
  for (int i = 0; i < 59; i++) {
    int currentIndex = (dataIndex + i) % 60;
    int nextIndex = (dataIndex + i + 1) % 60;
    
    int x1 = graphX + map(i, 0, 59, 0, graphWidth);
    int x2 = graphX + map(i + 1, 0, 59, 0, graphWidth);
    
    int y1 = graphY + graphHeight - map(graphData[currentIndex], 0, 1023, 0, graphHeight);
    int y2 = graphY + graphHeight - map(graphData[nextIndex], 0, 1023, 0, graphHeight);
    
    tft.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
  
  // Graph title
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(graphX, graphY - 10);
  tft.print("Light Level Over Time");
}

void checkButtons() {
  static unsigned long lastButtonPress = 0;
  
  if (millis() - lastButtonPress > 200) {
    if (!digitalRead(WIO_KEY_A)) {
      setpoint = constrain(setpoint - 25, 100, 900);
      lastButtonPress = millis();
      tone(WIO_BUZZER, 800, 50);
      integral = 0;
    }
    
    if (!digitalRead(WIO_KEY_B)) {
      setpoint = constrain(setpoint + 25, 100, 900);
      lastButtonPress = millis();
      tone(WIO_BUZZER, 1200, 50);
      integral = 0;
    }
    
    if (!digitalRead(WIO_KEY_C)) {
      // Cycle through PID presets
      static int tuneState = 0;
      tuneState = (tuneState + 1) % 4;
      
      switch(tuneState) {
        case 0: Kp = 2.0; Ki = 0.1; Kd = 0.5; break;
        case 1: Kp = 3.0; Ki = 0.2; Kd = 0.8; break;
        case 2: Kp = 5.0; Ki = 0.3; Kd = 1.2; break;
        case 3: Kp = 1.0; Ki = 0.05; Kd = 0.2; break;
      }
      
      integral = 0;
      lastButtonPress = millis();
      tone(WIO_BUZZER, 2000, 100);
      
      // Update PID display
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(1);
      tft.fillRect(10, 50, 80, 40, TFT_BLACK);
      tft.setCursor(10, 50);
      tft.print("Kp: "); tft.println(Kp, 1);
      tft.setCursor(10, 65);
      tft.print("Ki: "); tft.println(Ki, 2);
      tft.setCursor(10, 80);
      tft.print("Kd: "); tft.println(Kd, 1);
    }
  }
}
