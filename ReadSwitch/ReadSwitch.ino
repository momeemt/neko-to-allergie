#include "SevenSegmentLED.hpp"
#include "TactileSwitch.hpp"

auto ssled = SevenSegmentLED();
auto tsw_a2 = TactileSwitch(A2);
auto tsw_a3 = TactileSwitch(A3);
auto tsw_a6 = TactileSwitch(A6);
auto tsw_a7 = TactileSwitch(A7);

void setup() {
  ssled.init();
  tsw_a2.init();
  tsw_a3.init();
  tsw_a6.init();
  tsw_a7.init();
}

void loop() {
  static int digit[] = { 0, 0, 0, 0 };
  TactileSwitch *tsws[] = { &tsw_a2, &tsw_a3, &tsw_a6, &tsw_a7 };

  for (int i = 0; i < 4; i++) {
    tsws[i]->update();
    if (tsws[i]->is_just_pressed()) {
      digit[i]++;
      if (digit[i] >= 10) {
        digit[i] = 0;
      }
    }
    ssled.write(i, digit[i]);
  }
}
