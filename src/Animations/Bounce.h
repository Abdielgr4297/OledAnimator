/*
  Bounce.h
  A box (or pixel) bouncing around the screen edges.

  Usage:
    Bounce::setSize(8);
    animator.addAnimation(Bounce::run);

  Made by Atharva Phadnis
*/

#ifndef OLED_ANIM_BOUNCE_H
#define OLED_ANIM_BOUNCE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

namespace Bounce {

inline uint8_t &boxSize() {
  static uint8_t s = 8;
  return s;
}

inline uint8_t &speedX() {
  static uint8_t sx = 2;
  return sx;
}

inline uint8_t &speedY() {
  static uint8_t sy = 1;
  return sy;
}

inline bool &filled() {
  static bool f = true;
  return f;
}

inline void setSize(uint8_t size) { boxSize() = size ? size : 1; }
inline void setSpeed(uint8_t sx, uint8_t sy) {
  speedX() = sx ? sx : 1;
  speedY() = sy ? sy : 1;
}
inline void setFilled(bool f) { filled() = f; }

// Triangle wave: maps a rising counter into a 0..range ping-pong value.
inline int16_t pingPong(uint32_t value, int16_t range) {
  if (range <= 0) return 0;
  const uint32_t period = (uint32_t)range * 2UL;
  const uint32_t pos = value % period;
  return (pos < (uint32_t)range) ? (int16_t)pos : (int16_t)(period - pos);
}

// Animation callback.
inline void run(Adafruit_SSD1306 *display, uint32_t frame) {
  const int16_t size = (int16_t)boxSize();
  const int16_t maxX = display->width() - size;
  const int16_t maxY = display->height() - size;

  const int16_t x = pingPong(frame * speedX(), maxX);
  const int16_t y = pingPong(frame * speedY(), maxY);

  if (filled()) {
    display->fillRect(x, y, size, size, SSD1306_WHITE);
  } else {
    display->drawRect(x, y, size, size, SSD1306_WHITE);
  }
}

}  // namespace Bounce

#endif  // OLED_ANIM_BOUNCE_H
