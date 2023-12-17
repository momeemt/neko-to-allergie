#ifndef TACTILE_SWITCH_HPP
#define TACTILE_SWITCH_HPP

class TactileSwitch {
    private:
        const int THRESHOLD = 500;
        uint8_t _pin;
        int _analog_value;
        int _last_analog_value;

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
        }

        bool is_pressed() {
            return _analog_value >= THRESHOLD;
        }

        bool is_just_pressed() {
            return !(_last_analog_value >= THRESHOLD) && is_pressed();
        }
};

#endif
