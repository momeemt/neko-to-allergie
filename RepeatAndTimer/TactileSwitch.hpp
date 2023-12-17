#ifndef TACTILE_SWITCH_HPP
#define TACTILE_SWITCH_HPP

class TactileSwitch {
    private:
        const int THRESHOLD = 500;
        uint8_t _pin;
        int _analog_value;
        int _last_analog_value;
        unsigned long _last_pressed_time;

    public:
        TactileSwitch(uint8_t pin) {
            _pin = pin;
        }

        void init() {
            pinMode(_pin, INPUT);
        }

        void update() {
            _last_analog_value = _analog_value;
            _analog_value = analogRead(_pin);

            if (is_just_pressed()) {
                _last_pressed_time = millis();
            }
        }

        bool is_pressed() {
            return _analog_value >= THRESHOLD;
        }

        bool is_just_pressed() {
            return !(_last_analog_value >= THRESHOLD) && is_pressed();
        }

        bool is_keyrepeat() {
            return is_pressed() && (millis() - _last_pressed_time >= 1000);
        }
};

#endif
