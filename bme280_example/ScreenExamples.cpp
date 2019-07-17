#include "ScreenExamples.hpp"

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
  uint16_t partitions = (uint16_t) (1000 * alpha);
  screen.plotParametricGraph(
    lissajous_x,
    lissajous_y,
    0,
    2 * PI * alpha,
    partitions,
    graphRect2,
    screenRect2,
    erase ? WHITE : BLUE
  );

  n = n + 1 < N ? n + 1 : 0;
  if (n == 0) {
    erase = !erase;
  }
}

void reset() {
  a = 0;
  n = 0;
  erase = false;

  M5.Lcd.fillScreen(WHITE);
}
