/*
  TextScroll.ino
  Scrolls a message right-to-left and adds a custom animation on top.

  Wiring (I2C):
    OLED SDA -> A4 (Uno/Nano) or GPIO21 (ESP32)
    OLED SCL -> A5 (Uno/Nano) or GPIO22 (ESP32)

  Made by Atharva Phadnis
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OledAnimator.h>
#include <Animations/ScrollText.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OledAnimator animator(&display);

// A custom animation: a progress bar sweeping across the top.
void progressBar(Adafruit_SSD1306 *d, uint32_t frame) {
  const int16_t w = (int16_t)(frame % (uint32_t)d->width());
  d->drawRect(0, 0, d->width(), 8, SSD1306_WHITE);
  d->fillRect(0, 0, w, 8, SSD1306_WHITE);
}

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 not found"));
    for (;;) {
    }
  }

  animator.begin(30);

  ScrollText::setText("Made by Atharva Phadnis - OledAnimator");
  ScrollText::setTextSize(2);
  ScrollText::setY(24);
  ScrollText::setSpeed(3);
  animator.addAnimation(ScrollText::run);

  animator.addAnimation(progressBar);
}

void loop() {
  animator.update();
}
