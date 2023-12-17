#include "SevenSegmentLED.hpp"
#include "VariableResistance.hpp"

auto ssled = SevenSegmentLED();
auto vr = VR();

void setup() {
  ssled.init();
  vr.init();
}

void loop() {
  ssled.write(vr.read());
}
