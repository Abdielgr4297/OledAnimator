/*
  Fade.h
  Simulated fade in/out on a 1-bit display using dither patterns.

  The SSD1306 has no grayscale, so "brightness" is emulated by drawing
  a fraction of the pixels using an ordered 4x4 Bayer matrix.

  Usage:
    Fade::setArea(0, 0, 128, 64);
    animator.addAnimation(Fade::run);

  Made by Atharva Phadnis
*/

#ifndef OLED_ANIM_FADE_H
#define OLED_ANIM_FADE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

namespace Fade {

// 4x4 ordered dither matrix (values 0..15) stored in flash.
const uint8_t BAYER_4X4[16] PROGMEM = {
    0,  8,  2,  10,
    12, 4,  14, 6,
    3,  11, 1,  9,
    15, 7,  13, 5};

inline int16_t &areaX() { static int16_t v = 0; return v; }
inline int16_t &areaY() { static int16_t v = 0; return v; }
inline int16_t &areaW() { static int16_t v = 0; return v; }  // 0 = full width
inline int16_t &areaH() { static int16_t v = 0; return v; }  // 0 = full height

// Frames for one full fade in + fade out cycle.
inline uint16_t &cycleFrames() { static uint16_t v = 60; return v; }

inline void setArea(int16_t x, int16_t y, int16_t w, int16_t h) {
  areaX() = x; areaY() = y; areaW() = w; areaH() = h;
}
inline void setCycleFrames(uint16_t frames) { cycleFrames() = frames ? frames : 1; }

// Draw a rectangle at a given brightness level (0..16).
inline void drawLevel(Adafruit_SSD1306 *display, int16_t x0, int16_t y0,
                      int16_t w, int16_t h, uint8_t level) {
  if (level == 0) return;
  for (int16_t y = 0; y < h; y++) {
    for (int16_t x = 0; x < w; x++) {
      const uint8_t threshold =
          pgm_read_byte(&BAYER_4X4[((y & 3) << 2) | (x & 3)]);
      if (threshold < level) {
        display->drawPixel(x0 + x, y0 + y, SSD1306_WHITE);
      }
    }
  }
}

// Animation callback: fades the configured area in and out forever.
inline void run(Adafruit_SSD1306 *display, uint32_t frame) {
  const int16_t w = areaW() > 0 ? areaW() : display->width();
  const int16_t h = areaH() > 0 ? areaH() : display->height();

  const uint16_t period = cycleFrames();
  const uint16_t pos = (uint16_t)(frame % period);
  const uint16_t half = period / 2;

  // Triangle wave 0..16..0
  uint8_t level;
  if (pos < half) {
    level = (uint8_t)((uint32_t)pos * 16UL / (half ? half : 1));
  } else {
    level = (uint8_t)((uint32_t)(period - pos) * 16UL / (half ? half : 1));
  }
  if (level > 16) level = 16;

  drawLevel(display, areaX(), areaY(), w, h, level);
}

}  // namespace Fade

#endif  // OLED_ANIM_FADE_H
