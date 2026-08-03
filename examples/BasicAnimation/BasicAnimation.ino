/*
  BasicAnimation.ino
  Runs a bouncing box together with a fading panel — fully non-blocking.

  Wiring (I2C):
    OLED SDA -> A4 (Uno/Nano) or GPIO21 (ESP32)
    OLED SCL -> A5 (Uno/Nano) or GPIO22 (ESP32)

  Made by Atharva Phadnis
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OledAnimator.h>
#include <Animations/Bounce.h>
#include <Animations/Fade.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledAnimator animator(&display);

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 not found"));
    for (;;) {
      // Halt: nothing to animate without a display.
    }
  }

  animator.begin(30);  // 30 FPS

  // Fade a small panel on the left side.
  Fade::setArea(0, 0, 32, 64);
  Fade::setCycleFrames(60);
  animator.addAnimation(Fade::run);  // duration 0 = forever

  // Bouncing box across the whole screen.
  Bounce::setSize(10);
  Bounce::setSpeed(2, 1);
  animator.addAnimation(Bounce::run);
}

void loop() {
  animator.update();  // never blocks — other code can run here freely
}
