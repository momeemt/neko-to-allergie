#include "../utils/SevenSegmentLED.hpp"

void setup() {
  auto ssled = SevenSegmentLED();
  ssled.init();
}

void loop() {
  auto ssled = SevenSegmentLED();
  ssled.write(123);
}
