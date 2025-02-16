#ifndef INCLUDE_GUARD_HEADER_SCREEN_HPP
#define INCLUDE_GUARD_HEADER_SCREEN_HPP

#include <M5StickC.h>
#include <vector>

template <typename T>
struct Rect {
  const T left;
  const T right;
  const T bottom;
  const T top;
  Rect(T left, T right, T bottom, T top)
    : left(left), right(right), bottom(bottom), top(top) {}
};

struct Screen {
  const int16_t screenWidth;
  const int16_t screenHeight;
  Screen(int16_t screenWidth, int16_t screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {}
  void plotGraph(float (*f)(float), Rect<float> graphRect, Rect<int16_t> screenRect, uint16_t color);
  void plotParametricGraph(
    float (*f)(float),
    float (*g)(float),
    float t_min,
    float t_max,
    uint16_t partitions,
    Rect<float> graphRect,
    Rect<int16_t> screenRect,
    uint16_t color
  );
  void plotLineGraph(std::vector<float>& dataVector, Rect<int16_t> screenRect, uint16_t color);
};

#endif // INCLUDE_GUARD_HEADER_SCREEN_HPP
