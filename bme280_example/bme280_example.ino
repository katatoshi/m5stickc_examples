#include <M5StickC.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <vector>

#include "Screen.hpp"

#define DELAY_MS 100

// NOTE Adafruit BME280 Library `1.1.0`
Adafruit_BME280 bme;

const size_t maxLogSize = 120;

std::vector<float> temperatureLog;

std::vector<float> pressureLog;

std::vector<float> humidityLog;

Screen graphScreen(160, 80);

long loopTime = 0;

long startTime = 0;

enum ScreenMode { PRINT, TEMPERATURE, PRESSURE, HUMIDITY };

ScreenMode screenMode = PRINT;

void changeScreenMode() {
  switch (screenMode) {
    case PRINT: screenMode = TEMPERATURE; break;
    case TEMPERATURE: screenMode = PRESSURE; break;
    case PRESSURE: screenMode = HUMIDITY; break;
    case HUMIDITY: screenMode = PRINT; break;
  }
}

enum Interval { SECOND, TEN_SECOND, MINUTE };

Interval interval = SECOND;

void changeInterval() {
  switch (interval) {
    case SECOND: interval = TEN_SECOND; break;
    case TEN_SECOND: interval = MINUTE; break;
    case MINUTE: interval = SECOND; break;
  }
}

long getIntervalMillis() {
  switch (interval) {
    case SECOND: return 1000;
    case TEN_SECOND: return 10 * 1000;
    case MINUTE: return 60 * 1000;
  }
}

void setup() {
  M5.begin();
  Wire.begin(32,33);

  bme.begin();

  M5.Axp.ScreenBreath(32); // NOTE 0〜100？　M5StickC が出た当初は15段階だったはずだけど cf. https://pages.switch-science.com/letsiot/m5stickc/

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK, WHITE);
}

void loop() {
  loopTime = millis();
  if (startTime < loopTime - getIntervalMillis()) {
    readValues();

    switch (screenMode) {
      case PRINT: printValues(); break;
      case TEMPERATURE: plotTemperatureGraph(); break;
      case PRESSURE: plotPressureGraph(); break;
      case HUMIDITY: plotHumidityGraph(); break;
    }

    startTime = loopTime;
  }


   if (digitalRead(M5_BUTTON_RST) == LOW) {
    changeInterval();
    resetLog();
    readValues();
    screenMode = PRINT;
    printValues();
    while (digitalRead(M5_BUTTON_RST) == LOW);
  }

  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    changeScreenMode();
    switch (screenMode) {
      case PRINT: printValues(); break;
      case TEMPERATURE: plotTemperatureGraph(); break;
      case PRESSURE: plotPressureGraph(); break;
      case HUMIDITY: plotHumidityGraph(); break;
    }
    while (digitalRead(M5_BUTTON_HOME) == LOW);
  }

  delay(DELAY_MS);
}

void resetLog() {
  temperatureLog.clear();
  pressureLog.clear();
  humidityLog.clear();
}

void readValues() {
  float temperature = bme.readTemperature();
  Serial.print("temperature : ");
  Serial.print(temperature);
  Serial.print(" DegC, ");

  temperatureLog.push_back(temperature);
  if (maxLogSize < temperatureLog.size()) {
    temperatureLog.erase(temperatureLog.begin());
  }

  float pressure = bme.readPressure() / 100.0F;
  Serial.print("pressure : ");
  Serial.print(pressure);
  Serial.print(" hPa, ");

  pressureLog.push_back(pressure);
  if (maxLogSize < pressureLog.size()) {
    pressureLog.erase(pressureLog.begin());
  }

  float humidity = bme.readHumidity();
  Serial.print("humidity : ");
  Serial.print(humidity);
  Serial.print(" %RH, ");

  humidityLog.push_back(humidity);
  if (maxLogSize < humidityLog.size()) {
    humidityLog.erase(humidityLog.begin());
  }

  switch (interval) {
    case SECOND: Serial.println(" 1sec"); break;
    case TEN_SECOND: Serial.println(" 10sec"); break;
    case MINUTE: Serial.println(" 1min"); break;
  }
}

void printValues() {
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setCursor(0, 0, 2);

  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.print("Temperature ");
  M5.Lcd.print(temperatureLog.back());
  M5.Lcd.println(" DegC");

  M5.Lcd.setTextColor(DARKGREEN, WHITE);
  M5.Lcd.print("Pressure ");
  M5.Lcd.print(bme.readPressure() / 100.0F);
  M5.Lcd.println(" hPa");

  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.print("Humidity ");
  M5.Lcd.print(bme.readHumidity());
  M5.Lcd.println(" %RH");

  M5.Lcd.setTextColor(BLACK, WHITE);
  switch (interval) {
    case SECOND: M5.Lcd.println("Interval 1sec"); break;
    case TEN_SECOND: M5.Lcd.println("Interval 10sec"); break;
    case MINUTE: M5.Lcd.println("Interval 1min"); break;
  }
}

void plotTemperatureGraph() {
  M5.Lcd.fillScreen(WHITE);

  if (2 <= temperatureLog.size()) {
//    std::vector<float> dataVector(temperatureLog.begin(), temperatureLog.end());
//    Rect<int16_t> screenRect(0, 159, 0, 79);
    Rect<int16_t> screenRect(30, 159, 0, 69);
    graphScreen.plotLineGraph(temperatureLog, screenRect, BLACK);
  }

  const float minTemperature = *std::min_element(temperatureLog.begin(), temperatureLog.end());
  const float maxTemperature = *std::max_element(temperatureLog.begin(), temperatureLog.end());

  M5.Lcd.setCursor(159 - 60 - 24, 0, 1);
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.print(temperatureLog.back());
  M5.Lcd.print(" DegC");
  switch (interval) {
    case SECOND: M5.Lcd.println(" 1s"); break;
    case TEN_SECOND: M5.Lcd.println(" 10s"); break;
    case MINUTE: M5.Lcd.println(" 1m"); break;
  }

  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.println(maxTemperature);

  M5.Lcd.setCursor(0, 79 - 8, 1);
  M5.Lcd.setTextColor(RED, WHITE);
  M5.Lcd.println(minTemperature);
}

void plotPressureGraph() {
  M5.Lcd.fillScreen(WHITE);

  if (2 <= pressureLog.size()) {
//    std::vector<float> dataVector(pressureLog.begin(), pressureLog.end());
//    Rect<int16_t> screenRect(0, 159, 0, 79);
    Rect<int16_t> screenRect(44, 159, 0, 69);
    graphScreen.plotLineGraph(pressureLog, screenRect, BLACK);
  }

  const float minPressure = *std::min_element(pressureLog.begin(), pressureLog.end());
  const float maxPressure = *std::max_element(pressureLog.begin(), pressureLog.end());

  M5.Lcd.setCursor(159 - 66 - 24, 0, 1);
  M5.Lcd.setTextColor(DARKGREEN, WHITE);
  M5.Lcd.print(pressureLog.back());
  M5.Lcd.print(" hPa");
  switch (interval) {
    case SECOND: M5.Lcd.println(" 1s"); break;
    case TEN_SECOND: M5.Lcd.println(" 10s"); break;
    case MINUTE: M5.Lcd.println(" 1m"); break;
  }

  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.setTextColor(DARKGREEN, WHITE);
  M5.Lcd.println(maxPressure);

  M5.Lcd.setCursor(0, 79 - 8, 1);
  M5.Lcd.setTextColor(DARKGREEN, WHITE);
  M5.Lcd.println(minPressure);
}

void plotHumidityGraph() {
  M5.Lcd.fillScreen(WHITE);

  if (2 <= humidityLog.size()) {
//    std::vector<float> dataVector(humidityLog.begin(), humidityLog.end());
//    Rect<int16_t> screenRect(0, 159, 0, 79);
    Rect<int16_t> screenRect(30, 159, 0, 69);
    graphScreen.plotLineGraph(humidityLog, screenRect, BLACK);
  }

  const float minHumidity = *std::min_element(humidityLog.begin(), humidityLog.end());
  const float maxHumidity = *std::max_element(humidityLog.begin(), humidityLog.end());

  M5.Lcd.setCursor(159 - 54 - 24, 0, 1);
  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.print(humidityLog.back());
  M5.Lcd.print(" %RH");
  switch (interval) {
    case SECOND: M5.Lcd.println(" 1s"); break;
    case TEN_SECOND: M5.Lcd.println(" 10s"); break;
    case MINUTE: M5.Lcd.println(" 1m"); break;
  }

  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.println(maxHumidity);

  M5.Lcd.setCursor(0, 79 - 8, 1);
  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.println(minHumidity);
}
