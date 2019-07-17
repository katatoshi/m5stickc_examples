#include <M5StickC.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "ScreenExamples.hpp"

#define DELAY_MS 33

Adafruit_BME280 bme;

bool parametric = false;

void setup() {
  M5.begin();
  Wire.begin(32,33);

  bme.begin();

  M5.Axp.ScreenBreath(10);

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);

  reset();
}

void loop() {
  if (parametric) {
    plotParametricGraph();
  } else {
    plotGraph();
  }

  if (digitalRead(M5_BUTTON_RST) == LOW) {
    reset();
    while (digitalRead(M5_BUTTON_RST) == LOW);
  }

  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    parametric = !parametric;
    reset();
    while (digitalRead(M5_BUTTON_HOME) == LOW);
  }

  delay(DELAY_MS);
}

void printValues() {
  M5.Lcd.setCursor(0, 0, 2);

  M5.Lcd.print("Temperature ");
  M5.Lcd.print(bme.readTemperature());
  M5.Lcd.println(" DegC");

  M5.Lcd.print("Pressure ");
  M5.Lcd.print(bme.readPressure() / 100.0F);
  M5.Lcd.println(" hPa");

  M5.Lcd.print("Humidity ");
  M5.Lcd.print(bme.readHumidity());
  M5.Lcd.println(" %RH");
}
