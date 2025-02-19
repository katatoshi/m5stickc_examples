#include "Screen.hpp"

void Screen::plotGraph(float (*f)(float), Rect<float> graphRect, Rect<int16_t> screenRect, uint16_t color) {
  if (graphRect.right <= graphRect.left || graphRect.top <= graphRect.bottom) {
    return;
  }

  if (screenRect.right <= screenRect.left || screenRect.top <= screenRect.bottom) {
    return;
  }

  if (screenWidth - 1 <= screenRect.left || screenRect.right <= 0) {
    return;
  }

  if (screenHeight - 1 <= screenRect.bottom || screenRect.top <= 0) {
    return;
  }

  const float x_min = graphRect.left;
  const float x_max = graphRect.right;
  const float y_min = graphRect.bottom;
  const float y_max = graphRect.top;

  const int16_t u_min = screenRect.left < 0 ? 0 : screenRect.left;
  const int16_t u_max = screenWidth - 1 < screenRect.right ? screenWidth - 1 : screenRect.right;
  const int16_t v_min = screenRect.bottom < 0 ? 0 : screenRect.bottom;
  const int16_t v_max = screenHeight - 1 < screenRect.top ? screenHeight - 1 : screenRect.top;

  float a = ((float) (u_max - u_min)) / ((float) (x_max - x_min));
  float c = ((float) (v_max - v_min)) / ((float) (y_max - y_min));

  for (int16_t u = u_min; u <= u_max; u++) {
    int16_t v = (int16_t) (-(c * (f((u - u_min) / a + x_min) - y_min) + v_min) + screenHeight - 1);
    M5.Lcd.drawPixel(u, v, color);
  }
}

void Screen::plotParametricGraph(
  float (*f)(float),
  float (*g)(float),
  float t_min,
  float t_max,
  uint16_t partitions,
  Rect<float> graphRect,
  Rect<int16_t> screenRect,
  uint16_t color
) {
  if (t_max <= t_min) {
    return;
  }

  if (partitions <= 0) {
    return;
  }

  if (graphRect.right <= graphRect.left || graphRect.top <= graphRect.bottom) {
    return;
  }

  if (screenRect.right <= screenRect.left || screenRect.top <= screenRect.bottom) {
    return;
  }

  if (screenWidth - 1 <= screenRect.left || screenRect.right <= 0) {
    return;
  }

  if (screenHeight - 1 <= screenRect.bottom || screenRect.top <= 0) {
    return;
  }

  const float x_min = graphRect.left;
  const float x_max = graphRect.right;
  const float y_min = graphRect.bottom;
  const float y_max = graphRect.top;

  const int16_t u_min = screenRect.left < 0 ? 0 : screenRect.left;
  const int16_t u_max = screenWidth - 1 < screenRect.right ? screenWidth - 1 : screenRect.right;
  const int16_t v_min = screenRect.bottom < 0 ? 0 : screenRect.bottom;
  const int16_t v_max = screenHeight - 1 < screenRect.top ? screenHeight - 1 : screenRect.top;

  float a = ((float) (u_max - u_min)) / ((float) (x_max - x_min));
  float c = ((float) (v_max - v_min)) / ((float) (y_max - y_min));

  float increment = (t_max - t_min) / partitions;
  for (uint16_t i = 0; i < partitions; i++) {
    int16_t u = (int16_t) (a * (f(t_min + i * increment) - x_min) + u_min);
    if (u < u_min || u_max < u) {
      continue;
    }

    int16_t v = (int16_t) (c * (g(t_min + i * increment) - y_min) + v_min);
    if (v < v_min || v_max < v) {
      continue;
    }

    M5.Lcd.drawPixel(u, -v + screenHeight - 1, color);
  }
}

void Screen::plotLineGraph(std::vector<float>& dataVector, Rect<int16_t> screenRect, uint16_t color) {
  const std::size_t n = dataVector.size();
  if (n <= 1) {
    return;
  }

  if (screenRect.right <= screenRect.left || screenRect.top <= screenRect.bottom) {
    return;
  }

  if (screenWidth - 1 <= screenRect.left || screenRect.right <= 0) {
    return;
  }

  if (screenHeight - 1 <= screenRect.bottom || screenRect.top <= 0) {
    return;
  }

  const float x_min = 0;
  const float x_max = n - 1;
  const float y_min = *std::min_element(dataVector.begin(), dataVector.end());
  const float y_max = *std::max_element(dataVector.begin(), dataVector.end());

  const int16_t u_min = screenRect.left < 0 ? 0 : screenRect.left;
  const int16_t u_max = screenWidth - 1 < screenRect.right ? screenWidth - 1 : screenRect.right;
  const int16_t v_min = screenRect.bottom < 0 ? 0 : screenRect.bottom;
  const int16_t v_max = screenHeight - 1 < screenRect.top ? screenHeight - 1 : screenRect.top;

  float a = ((float) (u_max - u_min)) / ((float) (n - 1));
  float c = ((float) (v_max - v_min)) / ((float) (y_max - y_min));

  int16_t u = (int16_t) u_min;
  int16_t v = (int16_t) (c * (dataVector[0] - y_min) + v_min);
  for (std::size_t i = 1; i < n; i++) {
    int16_t u_next = (int16_t) (a * i + u_min);
    int16_t v_next = (int16_t) (c * (dataVector[i] - y_min) + v_min);
    M5.Lcd.drawLine(u, -v + screenHeight - 1, u_next, -v_next + screenHeight - 1, color);

    u = u_next;
    v = v_next;
  }
}
