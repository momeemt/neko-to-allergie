#include <TimerOne.h>
#include "pitches.h"
#include "SevenSegmentLED.hpp"
#include "PiezoelectricSounder.hpp"
#include "TactileSwitch.hpp"
#include "KitchenTimer.hpp"
#include "VariableResistance.hpp"
#include "BulletTypeLED.hpp"

auto ssled = SevenSegmentLED();
auto ps = PiezoelectricSounder();
auto vr = VR();
auto plus_tsw = TactileSwitch(A2);
auto minus_tsw = TactileSwitch(A3);
auto mode_tsw = TactileSwitch(A6);
auto timer_tsw = TactileSwitch(A7);
auto kitchen_timer = KitchenTimer();
auto bullet_type_led = BulletTypeLED();

void setup() {
  ssled.init();
  ps.init();
  vr.init();
  plus_tsw.init();
  minus_tsw.init();
  mode_tsw.init();
  timer_tsw.init();
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(countdown);
  Serial.begin(9600);
}

void countdown() {
  kitchen_timer.countdown_interrupt();
}

void loop() {
  kitchen_timer.set_count_change_interval(vr.read() / 1.5);
  plus_tsw.update();
  minus_tsw.update();
  mode_tsw.update();
  timer_tsw.update();
  kitchen_timer.update(plus_tsw, minus_tsw, mode_tsw, timer_tsw, ps, bullet_type_led);
  ssled.write(kitchen_timer.get_time_for_display());
}