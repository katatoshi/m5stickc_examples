#include <M5StickC.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "Screen.hpp"

#define DELAY_MS 33

Adafruit_BME280 bme;

void setup() {
  M5.begin();
  Wire.begin(32,33);

  bme.begin();

  M5.Axp.ScreenBreath(10);

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
}

void loop() {
  plotParametricGraph();
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

Screen screen(160, 80);

int a = 0;

float float_sin(float x) {
  return (float) sin(x + a / 10.0);
}

float float_sqrt(float x) {
  return (float) sqrt(x);
}

float float_pow(float x) {
  return (float) pow(x, (3 * sin(a /20.0) + 5) / 4);
}

float float_cos(float x) {
  return (float) cos(x + a / 5.0);
}

float lissajous_x(float t) {
  return (float) cos(3 * t);
}

float lissajous_y(float t) {
  return (float) sin(4 * t);
}

void plotGraph() {
  M5.Lcd.fillScreen(LIGHTGREY);

  Rect<float> graphRect1(0, 4 * PI, -1, 1);
  Rect<float> graphRect2(0, 1, 0, 1);
  Rect<float> graphRect3(0, 1, 0, 1);
  Rect<float> graphRect4(0, 4 * PI, -1, 1);

  Rect<int16_t> screenRect1(0, 79, 0, 39);
  Rect<int16_t> screenRect2(80, 159, 0, 39);
  Rect<int16_t> screenRect3(0, 79, 40, 79);
  Rect<int16_t> screenRect4(80, 159, 40, 79);

  screen.plotGraph(float_sin, graphRect1, screenRect1, RED);
  screen.plotGraph(float_sqrt, graphRect2, screenRect2, GREEN);
  screen.plotGraph(float_pow, graphRect3, screenRect3, YELLOW);
  screen.plotGraph(float_cos, graphRect4, screenRect4, BLUE);

  a++;
}

#define N 100

int n = 0;

bool erase = false;

void plotParametricGraph() {
  Rect<float> graphRect1(-1, 1, -1, 1);
  Rect<int16_t> screenRect1(0, 79, 0, 79);
  screen.plotParametricGraph(float_cos, float_sin, 0, 2 * PI, 500, graphRect1, screenRect1, RED);

  Rect<float> graphRect2(-1, 1, -1, 1);
  Rect<int16_t> screenRect2(80, 159, 0, 79);
  float alpha = (float) n / (float) N;
  screen.plotParametricGraph(
    lissajous_x,
    lissajous_y,
    0,
    2 * PI * alpha,
    (uint16_t) (1000 * alpha),
    graphRect2,
    screenRect2,
    erase ? WHITE : BLUE
  );

  n = n + 1 < N ? n + 1 : 0;
  if (n == 0) {
    erase = !erase;
  }
}
