#include <M5StickC.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define DELAY_MS 1000

Adafruit_BME280 bme;

void setup() {
  M5.begin();
  Wire.begin(32,33);

  bme.begin();

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
}

void loop() {
  printValues();
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
