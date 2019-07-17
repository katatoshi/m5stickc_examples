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

  float a = (u_max - u_min) / (x_max - x_min);
  float c = (v_max - v_min) / (y_max - y_min);

  for (int16_t u = u_min; u <= u_max; u++) {
    int16_t v = (int16_t) (-(c * (f((u - u_min) / a + x_min) - y_min) + v_min) + screenHeight - 1);
    M5.Lcd.drawPixel(u, v, color);
  }
}
