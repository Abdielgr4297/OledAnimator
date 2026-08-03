/*
  OledAnimator.cpp
  Implementation of the OledAnimator engine.

  Made by Atharva Phadnis
*/

#include "OledAnimator.h"

OledAnimator::OledAnimator(Adafruit_SSD1306 *display)
    : _display(display),
      _frameInterval(1000UL / OLED_ANIM_DEFAULT_FPS),
      _lastFrameTime(0) {
  for (uint8_t i = 0; i < OLED_ANIM_MAX_ANIMATIONS; i++) {
    _slots[i].callback = NULL;
    _slots[i].duration = 0;
    _slots[i].startTime = 0;
    _slots[i].frame = 0;
    _slots[i].active = false;
  }
}

void OledAnimator::begin(uint8_t fps) {
  setFPS(fps);
  _lastFrameTime = millis();
  if (_display) {
    _display->clearDisplay();
    _display->display();
  }
}

void OledAnimator::setFPS(uint8_t fps) {
  if (fps == 0) fps = 1;
  if (fps > 120) fps = 120;
  _frameInterval = 1000UL / fps;
}

int8_t OledAnimator::addAnimation(AnimationCallback callback, uint32_t duration) {
  if (callback == NULL) return -1;

  for (uint8_t i = 0; i < OLED_ANIM_MAX_ANIMATIONS; i++) {
    if (!_slots[i].active) {
      _slots[i].callback = callback;
      _slots[i].duration = duration;
      _slots[i].startTime = millis();
      _slots[i].frame = 0;
      _slots[i].active = true;
      return (int8_t)i;
    }
  }
  return -1;  // no free slot
}

bool OledAnimator::stopAnimation(uint8_t index) {
  if (index >= OLED_ANIM_MAX_ANIMATIONS) return false;
  if (!_slots[index].active) return false;

  _slots[index].active = false;
  _slots[index].callback = NULL;
  return true;
}

void OledAnimator::stopAll() {
  for (uint8_t i = 0; i < OLED_ANIM_MAX_ANIMATIONS; i++) {
    _slots[i].active = false;
    _slots[i].callback = NULL;
  }
  clear();
}

uint8_t OledAnimator::activeCount() const {
  uint8_t n = 0;
  for (uint8_t i = 0; i < OLED_ANIM_MAX_ANIMATIONS; i++) {
    if (_slots[i].active) n++;
  }
  return n;
}

void OledAnimator::clear() {
  if (!_display) return;
  _display->clearDisplay();
  _display->display();
}

bool OledAnimator::update() {
  if (!_display) return false;

  const uint32_t now = millis();
  // millis() overflow-safe comparison.
  if ((uint32_t)(now - _lastFrameTime) < _frameInterval) return false;
  _lastFrameTime = now;

  _display->clearDisplay();

  bool drewSomething = false;
  for (uint8_t i = 0; i < OLED_ANIM_MAX_ANIMATIONS; i++) {
    AnimationSlot &slot = _slots[i];
    if (!slot.active || slot.callback == NULL) continue;

    // Expire timed animations.
    if (slot.duration > 0 && (uint32_t)(now - slot.startTime) >= slot.duration) {
      slot.active = false;
      slot.callback = NULL;
      continue;
    }

    slot.callback(_display, slot.frame);
    slot.frame++;
    drewSomething = true;
  }

  _display->display();
  return drewSomething;
}
