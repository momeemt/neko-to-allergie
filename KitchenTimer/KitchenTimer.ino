#include <TimerOne.h>
#include "pitches.h"
#include "SevenSegmentLED.hpp"
#include "PiezoelectricSounder.hpp"
#include "TactileSwitch.hpp"

typedef enum kitchen_timer_mode_kind {
  KTMK_SETTING_TIMER,
  KTMK_COUNTDOWN,
  KTMK_BEEP,
} KitchenTimerModeKind;

KitchenTimerModeKind kitchen_timer_mode = KTMK_SETTING_TIMER;
unsigned int timecount = 0;  // 単位は秒
unsigned int start_countdown_time = 0;
bool started_beep = false;

// resは要素数4の配列である必要がある
void toMinutesAndSeconds(byte *res) {
  res[3] = (timecount / 60) / 10;
  res[2] = (timecount / 60) % 10;
  res[1] = (timecount % 60) / 10;
  res[0] = (timecount % 60) % 10;
}

auto ssled = SevenSegmentLED();
auto ps = PiezoelectricSounder();
auto plus_tsw = TactileSwitch(A2);
auto minus_tsw = TactileSwitch(A3);
auto mode_tsw = TactileSwitch(A6);

void setup() {
  ssled.init();
  ps.init();
  set_melody();
  plus_tsw.init();
  minus_tsw.init();
  mode_tsw.init();
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(count_down);
}

void set_melody() {
  for (int i = 0; i < 32; i++) {
    if (i % 2 == 0) {
      ps.add_note(NOTE_D5, 150);
    } else if (i % 8 == 7) {
      ps.add_note(NULL, 200);
    } else {
      ps.add_note(NULL, 30);
    }
  }
}

void beep() {
  ps.play();
  kitchen_timer_mode = KTMK_SETTING_TIMER;
  started_beep = false;
}

void count_down() {
  if (kitchen_timer_mode == KTMK_COUNTDOWN && timecount > 0 && millis() - start_countdown_time >= 1000) {
    timecount--;
  }
  if (kitchen_timer_mode == KTMK_COUNTDOWN && timecount == 0) {
    kitchen_timer_mode = KTMK_BEEP;
  }
}

void setting_mode_process() {
  const int MAX_TIMECOUNT = 59 * 60 + 59;
  unsigned long now = millis();
  static unsigned long last_count_change_time = 0;

  if (plus_tsw.is_just_pressed() && timecount < MAX_TIMECOUNT) {
    timecount++;
  }
  if (plus_tsw.is_keyrepeat()) {
    if (now - last_count_change_time >= 100 && timecount < MAX_TIMECOUNT) {
      timecount++;
      last_count_change_time = now;
    }
  }

  if (minus_tsw.is_just_pressed() && timecount > 0) {
    timecount--;
  }
  if (minus_tsw.is_keyrepeat()) {
    if (now - last_count_change_time >= 100 && timecount > 0) {
      timecount--;
      last_count_change_time = now;
    }
  }

  if (mode_tsw.is_just_pressed() && timecount > 0) {
    kitchen_timer_mode = KTMK_COUNTDOWN;
    start_countdown_time = millis();
  }
}

void countdown_mode_process() {
  if (mode_tsw.is_just_pressed()) {
    kitchen_timer_mode = KTMK_SETTING_TIMER;
  }
}

void loop() {
  plus_tsw.update();
  minus_tsw.update();
  mode_tsw.update();

  switch (kitchen_timer_mode) {
    case KTMK_SETTING_TIMER:
      setting_mode_process();
      break;
    case KTMK_COUNTDOWN:
      countdown_mode_process();
      break;
    case KTMK_BEEP:
      if (!started_beep) {
        beep();
        started_beep = true;
      }
      break;
  }

  byte digit[4];
  toMinutesAndSeconds(digit);
  ssled.write(digit);
}
