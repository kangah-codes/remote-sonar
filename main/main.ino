#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Servo.h>
U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 9, /* cs=*/ 12, /* dc=*/ 11, /* reset=*/ 13);  // Nokia 5110 Display
Servo Servo1;
const int servoPin = 3;
//for sonar
const int triggerPin = 4;
const int echoPin = 5;
int distance;
int locationOfObjects[180];

void drawDial(int angle) {
  u8g2.drawCircle(64, 16, 32, U8G2_DRAW_ALL);//center(42,48) radius:41 - 32
  u8g2.drawCircle(64, 16, 24, U8G2_DRAW_ALL);//center(42,48) radius:31 - 24 
  u8g2.drawCircle(64, 17, 16, U8G2_DRAW_ALL);//center(42,48) radius:21 - 16
  u8g2.drawCircle(64, 16, 8, U8G2_DRAW_ALL);//center(42,48) radius:11 - 8
  int x = 64 - 32 *  cos(angle * 3.14 / 180);
  int y = 16 - 32 *  sin(angle * 3.14 / 180);
  u8g2.drawLine(42, 48, x, y);
}

void drawObjectLine(int value, int angle) {
  int x0 = 64 - 32 *  cos(angle * 3.14 / 180);
  int y0 = 16 - 32 *  sin(angle * 3.14 / 180);
  int x1 = 64 - value *  cos(angle * 3.14 / 180);
  int y1 = 16 - value *  sin(angle * 3.14 / 180);
  u8g2.drawLine(x1, y1, x0, y0);
}

int lol() {
  long duration;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return duration * 0.034 / 2;
}
  
void clearArray() {
  for (int i = 0; i < 180; i++) {
    locationOfObjects[i] = 0;
  }
}

void setup(void) {
  u8g2.begin();
  Servo1.attach(servoPin);
  Serial.begin(9600);

  Serial.println("LMAO");
  for (int i = 0; i < 180; i++) {
    Servo1.write(i);
    u8g2.clearBuffer();
    drawDial(i);
    distance = random(300);
    if (distance < 41) {
      locationOfObjects[i] = distance;
    } else {
      locationOfObjects[i] = 0;
    }
    for (int k = 0; k < i; k++) {
      if (locationOfObjects[k]) {
        drawObjectLine(locationOfObjects[k], k);
      }
    }
    u8g2.sendBuffer();
    delay(10);
  }

  clearArray();

  for (int i = 180; i > 0; i--) {
    Servo1.write(i);
    u8g2.clearBuffer();
    drawDial(i);
    distance = random(300);
    if (distance < 41) {
      locationOfObjects[i] = distance;
    } else {
      locationOfObjects[i] = 0;
    }
    for (int k = 180; k > i; k--) {
      if (locationOfObjects[k]) {
        drawObjectLine(locationOfObjects[k], k);
      }
    }
    u8g2.sendBuffer();
    delay(10);
  }
  clearArray();
}

void loop(void) {
  
}
