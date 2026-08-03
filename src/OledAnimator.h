/*
  OledAnimator.h
  Lightweight, non-blocking animation engine for SSD1306 OLED displays.

  - No delay(), timing driven by millis()
  - No dynamic memory allocation (fixed-size slot array)
  - Up to OLED_ANIM_MAX_ANIMATIONS concurrent animations

  Made by Atharva Phadnis
*/

#ifndef OLED_ANIMATOR_H
#define OLED_ANIMATOR_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

// Maximum number of animations running at the same time.
#define OLED_ANIM_MAX_ANIMATIONS 5

// Default target frame rate (frames per second).
#define OLED_ANIM_DEFAULT_FPS 30

// Animation callback signature.
// display : pointer to the display being drawn on
// frame   : frame counter for this animation (starts at 0)
typedef void (*AnimationCallback)(Adafruit_SSD1306 *display, uint32_t frame);

// One animation slot. Plain struct, stored statically inside OledAnimator.
struct AnimationSlot {
  AnimationCallback callback;  // drawing function (NULL = free slot)
  uint32_t duration;           // duration in ms (0 = run forever)
  uint32_t startTime;          // millis() when the animation was added
  uint32_t frame;              // frames drawn so far
  bool active;                 // slot in use?
};

class OledAnimator {
 public:
  // display : pointer to an already-constructed Adafruit_SSD1306 object
  explicit OledAnimator(Adafruit_SSD1306 *display);

  // Prepare the engine. Call after display->begin().
  // fps : target frame rate (1..120)
  void begin(uint8_t fps = OLED_ANIM_DEFAULT_FPS);

  // Call as often as possible from loop(). Non-blocking.
  // Returns true if a frame was actually rendered.
  bool update();

  // Clear the display buffer and push it to the screen.
  void clear();

  // Register an animation.
  // duration : milliseconds to run, 0 = run forever.
  // Returns the slot index, or -1 if no slot is free.
  int8_t addAnimation(AnimationCallback callback, uint32_t duration = 0);

  // Remove every running animation and clear the screen.
  void stopAll();

  // Remove a single animation by slot index.
  bool stopAnimation(uint8_t index);

  // Number of animations currently running.
  uint8_t activeCount() const;

  // Change the target frame rate at runtime.
  void setFPS(uint8_t fps);

  // Access the underlying display (handy inside sketches).
  Adafruit_SSD1306 *display() const { return _display; }

 private:
  Adafruit_SSD1306 *_display;
  AnimationSlot _slots[OLED_ANIM_MAX_ANIMATIONS];
  uint32_t _frameInterval;  // ms between frames
  uint32_t _lastFrameTime;  // millis() of last rendered frame
};

#endif  // OLED_ANIMATOR_H
