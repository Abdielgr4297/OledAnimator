# OledAnimator

Create smooth, multi-layered OLED animations on Arduino — without delay(), without lag, and without wasting RAM.

OledAnimator lets you run several frame-based animations at once without ever calling `delay()`. Animations are plain function pointers, so adding your own takes three lines of code.

## Features

- Non-blocking: timing driven entirely by `millis()`, never `delay()`
- Up to 5 simultaneous animations
- Function-pointer animation callbacks — trivially extensible
- Frame-based updates with configurable FPS (default 30)
- Optional per-animation duration (auto-stop) or run-forever mode
- Zero dynamic memory allocation (no `malloc`, no `new`)
- Built-in animations: ScrollText, Bounce, Fade
- Works with any `Adafruit_SSD1306` display (I2C or SPI)

## Requirements

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

## Installation

### Arduino IDE (ZIP)
1. Download this repository as a ZIP.
2. Arduino IDE -> **Sketch > Include Library > Add .ZIP Library...**
3. Select the ZIP file.
4. Install **Adafruit SSD1306** and **Adafruit GFX Library** from the Library Manager.

### Manual
Copy the `OledAnimator` folder into your Arduino `libraries/` directory and restart the IDE.

## Example usage

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OledAnimator.h>
#include <Animations/ScrollText.h>
#include <Animations/Bounce.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
OledAnimator animator(&display);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  animator.begin(30);                 // 30 FPS

  ScrollText::setText("Hello World");
  animator.addAnimation(ScrollText::run);       // runs forever

  Bounce::setSize(8);
  animator.addAnimation(Bounce::run, 5000);     // stops after 5 seconds
}

void loop() {
  animator.update();                  // never blocks
}
```

### Writing your own animation

```cpp
void myAnim(Adafruit_SSD1306 *d, uint32_t frame) {
  d->drawCircle(64, 32, frame % 30, SSD1306_WHITE);
}

animator.addAnimation(myAnim);
```

## API

| Method | Description |
| --- | --- |
| `OledAnimator(Adafruit_SSD1306 *display)` | Construct with a pointer to your display |
| `begin(uint8_t fps = 30)` | Initialise the engine and set the frame rate |
| `update()` | Render a frame if it is due; call from `loop()` |
| `clear()` | Clear the buffer and the screen |
| `addAnimation(callback, duration = 0)` | Register an animation, `0` = forever; returns slot index or `-1` |
| `stopAnimation(index)` | Remove one animation |
| `stopAll()` | Remove every animation and clear the screen |
| `activeCount()` | Number of running animations |
| `setFPS(fps)` | Change frame rate at runtime |

Animation callback signature:

```cpp
void anim(Adafruit_SSD1306 *display, uint32_t frame);
```

## Built-in animations

| Animation | Header | Description |
| --- | --- | --- |
| ScrollText | `Animations/ScrollText.h` | Scrolls text right-to-left with configurable size, Y position and speed |
| Bounce | `Animations/Bounce.h` | A filled or outlined box bouncing off the screen edges |
| Fade | `Animations/Fade.h` | Simulated fade in/out using a 4x4 ordered dither pattern |

## Examples

- `examples/BasicAnimation/BasicAnimation.ino` — bouncing box plus a fading panel
- `examples/TextScroll/TextScroll.ino` — scrolling text plus a custom progress bar


## overview
<img width="1280" height="720" alt="WIN_20260804_18_03_37_Pro" src="https://github.com/user-attachments/assets/cf9812c0-21a5-47f9-a5fa-68c8eda599d3" />

## Why it's lightweight

- **No heap usage.** Animation slots live in a fixed-size array inside the object, so there is no fragmentation and RAM usage is known at compile time (~60 bytes for 5 slots).
- **No virtual classes.** Animations are raw function pointers, avoiding vtables and inheritance overhead.
- **Header-only animations.** Unused animations are never linked into the binary.
- **Dither tables in flash.** Constant data uses `PROGMEM`, keeping SRAM free.
- **One display flush per frame.** The engine clears, draws every animation, and pushes the buffer exactly once.
- **No extra dependencies** beyond Adafruit GFX and SSD1306.

## License

MIT — see [LICENSE](LICENSE).

---

Made by Atharva Phadnis
