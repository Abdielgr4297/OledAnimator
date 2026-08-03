/*
  ScrollText.h
  Scrolls a text string from right to left.

  Usage:
    ScrollText::setText("Hello World");
    animator.addAnimation(ScrollText::run);

  Made by Atharva Phadnis
*/

#ifndef OLED_ANIM_SCROLLTEXT_H
#define OLED_ANIM_SCROLLTEXT_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

namespace ScrollText {

// Fixed buffer, no dynamic allocation.
#ifndef SCROLLTEXT_MAX_LEN
#define SCROLLTEXT_MAX_LEN 48
#endif

inline char *buffer() {
  static char buf[SCROLLTEXT_MAX_LEN] = "OledAnimator";
  return buf;
}

inline uint8_t &textSize() {
  static uint8_t size = 1;
  return size;
}

inline int16_t &yPosition() {
  static int16_t y = 24;
  return y;
}

inline uint8_t &speed() {  // pixels moved per frame
  static uint8_t px = 2;
  return px;
}

// Set the scrolling text (copied into a fixed buffer).
inline void setText(const char *text) {
  if (text == NULL) return;
  strncpy(buffer(), text, SCROLLTEXT_MAX_LEN - 1);
  buffer()[SCROLLTEXT_MAX_LEN - 1] = '\0';
}

inline void setTextSize(uint8_t size) { textSize() = size ? size : 1; }
inline void setY(int16_t y) { yPosition() = y; }
inline void setSpeed(uint8_t pixelsPerFrame) { speed() = pixelsPerFrame ? pixelsPerFrame : 1; }

// Animation callback.
inline void run(Adafruit_SSD1306 *display, uint32_t frame) {
  const uint8_t size = textSize();
  const int16_t charW = 6 * size;  // default Adafruit font is 5x7 + 1px spacing
  const int16_t textW = (int16_t)strlen(buffer()) * charW;
  const int16_t screenW = display->width();
  const int16_t span = screenW + textW;

  const int16_t x = screenW - (int16_t)((frame * speed()) % (uint32_t)span);

  display->setTextSize(size);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(x, yPosition());
  display->print(buffer());
}

}  // namespace ScrollText

#endif  // OLED_ANIM_SCROLLTEXT_H
