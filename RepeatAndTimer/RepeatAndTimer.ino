#include <TimerOne.h>
#include "SevenSegmentLED.hpp"
#include "TactileSwitch.hpp"

unsigned int count = 0;
auto ssled = SevenSegmentLED();
auto tsw_a2 = TactileSwitch(A2);

void setup() {
  ssled.init();
  tsw_a2.init();

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(count_up);
}

void count_up() {
  count++;
}

void loop() {
  unsigned long now = millis();
  static unsigned long last_countup_time = 0;
  tsw_a2.update();

  if (tsw_a2.is_just_pressed()) {
    count++;
  }

  if (tsw_a2.is_keyrepeat()) {
    if (now - last_countup_time >= 100) {
      count++;
      last_countup_time = now;
    }
  }

  ssled.write(count);
}
