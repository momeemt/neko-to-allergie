#ifndef KITCHEN_TIMER_HPP
#define KITCHEN_TIMER_HPP

#include "TactileSwitch.hpp"
#include "PiezoelectricSounder.hpp"
#include "BulletTypeLED.hpp"
#include "Queue.h"
#include <StackArray.h>

typedef enum kitchen_timer_mode_kind {
  KTMK_SETTING_TIMER,
  KTMK_COUNTDOWN,
  KTMK_BEEP,
} KitchenTimerModeKind;

class KitchenTimer {
  private:
    static const int LEDS_NUMBER = 8;
    int _times[LEDS_NUMBER];
    KitchenTimerModeKind _modes[LEDS_NUMBER];
    unsigned long _last_count_change_times[LEDS_NUMBER] = {};
    unsigned long _start_countdown_times[LEDS_NUMBER] = {};
    bool _started_beeps[LEDS_NUMBER] = {};
    unsigned long _count_change_interval = 100;
    Queue<byte> _alarm_queue = Queue<byte>();
    StackArray<byte> _origin_led_index = StackArray<byte>();
    int _led_index = 0;

    void _setting_mode_process(TactileSwitch &plus_tsw, TactileSwitch &minus_tsw, TactileSwitch &mode_tsw) {
      const int MAX_TIMECOUNT = 59 * 60 + 59;
      unsigned long now = millis();

      if (plus_tsw.is_just_pressed() && _times[_led_index] < MAX_TIMECOUNT) {
        _times[_led_index]++;
      }
      if (plus_tsw.is_keyrepeat()) {
        if (now - _last_count_change_times[_led_index] >= _count_change_interval && _times[_led_index] < MAX_TIMECOUNT) {
          _times[_led_index]++;
          _last_count_change_times[_led_index] = now;
        }
      }

      if (minus_tsw.is_just_pressed() && _times[_led_index] > 0) {
        _times[_led_index]--;
      }
      if (minus_tsw.is_keyrepeat()) {
        if (now - _last_count_change_times[_led_index] >= _count_change_interval && _times[_led_index] > 0) {
          _times[_led_index]--;
          _last_count_change_times[_led_index] = now;
        }
      }

      if (mode_tsw.is_just_pressed() && _times[_led_index] > 0) {
        _modes[_led_index] = KTMK_COUNTDOWN;
        _start_countdown_times[_led_index] = millis();
      }
    }

    void _countdown_mode_process(TactileSwitch &mode_tsw) {
      if (mode_tsw.is_just_pressed()) {
        _modes[_led_index] = KTMK_SETTING_TIMER;
      }
    }

    void _set_melody(PiezoelectricSounder &ps, int index) {
      const unsigned int LED_NOTES[8] = {
        NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, 
        NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
      };
      ps.clear();
      for (int i = 0; i < 32; i++) {
        if (i % 2 == 0) {
          ps.add_note(LED_NOTES[index], 150);
        } else if (i % 8 == 7) {
          ps.add_note(NULL, 200);
        } else {
          ps.add_note(NULL, 30);
        }
      }
    }
  
  public:
    KitchenTimer() {
      for (int i = 0; i < LEDS_NUMBER; i++) {
        _times[i] = 0;
        _modes[i] = KTMK_SETTING_TIMER;
      }
    }

    void set_time(int time) {
      _times[_led_index] = time;
    }

    int get_time_for_display() {
      int minutes = _times[_led_index] / 60;
      int seconds = _times[_led_index] % 60;
      return minutes * 100 + seconds;
    }

    void countdown_interrupt() {
      for (int i = 0; i < LEDS_NUMBER; i++) {
        if (_modes[i] == KTMK_COUNTDOWN && _times[i] > 0 && millis() - _start_countdown_times[i] >= 1000) {
          _times[i]--;
        }
        if (_modes[i] == KTMK_COUNTDOWN && _times[i] == 0) {
          _origin_led_index.push(_led_index);
          _led_index = i;
          _alarm_queue.push(i);
          _modes[i] = KTMK_BEEP;
        }
      }
    }

    bool is_time_up() {
      return _times[_led_index] == 0;
    }

    void reset() {
      _times[_led_index] = 0;
    }

    KitchenTimerModeKind get_mode() {
      return _modes[_led_index];
    }

    void set_mode(KitchenTimerModeKind mode) {
      _modes[_led_index] = mode;
    }

    void update(
      TactileSwitch &plus_tsw,
      TactileSwitch &minus_tsw,
      TactileSwitch &mode_tsw,
      TactileSwitch &timer_tsw,
      PiezoelectricSounder &ps,
      BulletTypeLED &bullet_type_led
    ) {
      unsigned long now = millis();
      if (timer_tsw.is_just_pressed()) {
        _led_index = (_led_index + 1) % LEDS_NUMBER;
      }
      if (timer_tsw.is_keyrepeat()) {
        if (now - _last_count_change_times[_led_index] >= _count_change_interval) {
           _led_index = (_led_index + 1) % LEDS_NUMBER;
          _last_count_change_times[_led_index] = now;
        }
      }

      switch (_modes[_led_index]) {
        case KTMK_SETTING_TIMER:
          _setting_mode_process(plus_tsw, minus_tsw, mode_tsw);
          break;
        case KTMK_COUNTDOWN:
          _countdown_mode_process(mode_tsw);
          break;
        case KTMK_BEEP:
          if (_alarm_queue.count() != 0 && !_started_beeps[_led_index]) {
            _started_beeps[_led_index] = true;
            while (_alarm_queue.count() != 0) {
              int timer_index = _alarm_queue.pop();
              Serial.print("timer_index: ");
              Serial.println(timer_index);
              Serial.print("queue: ");
              Serial.println(_alarm_queue.count());
              bullet_type_led.light_up(timer_index);
              _set_melody(ps, timer_index);
              ps.play();
              _modes[timer_index] = KTMK_SETTING_TIMER;
              _started_beeps[timer_index] = false;
              _led_index = _origin_led_index.pop();
            }
            bullet_type_led.light_up(_led_index);
          }
          break;
      }

      bullet_type_led.light_up(_led_index);
    }

    void set_count_change_interval(unsigned long interval) {
      _count_change_interval = interval;
    }

    int get_led_index() {
      return _led_index;
    }
};

#endif