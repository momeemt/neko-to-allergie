#ifndef BULLET_TYPE_LED_HPP
#define BULLET_TYPE_LED_HPP

class BulletTypeLED {
  private:
    const byte LED_DRIVER_CLOCK = 14;
    const byte DATA_IO_PIN = 12;
    const byte FIXED_ADDRESS_MODE = 0b01000100;
    const byte FIRST_DIGIT_ADDRESS = 0b11000000;
    const byte PULSE_WIDTH[8] = {
        0b10001111,
        0b10001110,
        0b10001101,
        0b10001100,
        0b10001011,
        0b10001010,
        0b10001001,
        0b10001000
    };
    const byte HALF_CLOCK = 10;

    bool _write_serial_byte(byte b) {
        byte data = b;
        for (byte i = 0; i < 8; i++) {
            pinMode(LED_DRIVER_CLOCK, OUTPUT);
            delayMicroseconds(HALF_CLOCK);

            if (data & 0b00000001) {
                pinMode(DATA_IO_PIN, INPUT);
            } else {
                pinMode(DATA_IO_PIN, OUTPUT);
            }

            delayMicroseconds(HALF_CLOCK);
            pinMode(LED_DRIVER_CLOCK, INPUT);
            delayMicroseconds(HALF_CLOCK);
            data >>= 1;
        }
        pinMode(LED_DRIVER_CLOCK, OUTPUT);
        pinMode(DATA_IO_PIN, INPUT);
        delayMicroseconds(HALF_CLOCK);
        pinMode(LED_DRIVER_CLOCK, INPUT);
        delayMicroseconds(HALF_CLOCK);
        byte ack = digitalRead(DATA_IO_PIN);
        if (ack == 0) {
            pinMode(DATA_IO_PIN, OUTPUT);
        }
        delayMicroseconds(HALF_CLOCK);
        pinMode(LED_DRIVER_CLOCK, OUTPUT);
        delayMicroseconds(HALF_CLOCK);
        return ack;
    }

    void _start_condition() {
        pinMode(LED_DRIVER_CLOCK, INPUT);
        delayMicroseconds(HALF_CLOCK);
        pinMode(DATA_IO_PIN, INPUT);
        delayMicroseconds(HALF_CLOCK);
        pinMode(DATA_IO_PIN, OUTPUT);
        delayMicroseconds(HALF_CLOCK);
    }

    void _end_condition() {
        pinMode(DATA_IO_PIN, OUTPUT);
        delayMicroseconds(HALF_CLOCK);
        pinMode(LED_DRIVER_CLOCK, INPUT);
        delayMicroseconds(HALF_CLOCK);
        pinMode(DATA_IO_PIN, INPUT);
        delayMicroseconds(HALF_CLOCK);
    }

    void _led8_set(byte data, byte intensity) {
        _start_condition();
        _write_serial_byte(FIXED_ADDRESS_MODE);
        _end_condition();

        _start_condition();
        _write_serial_byte(FIRST_DIGIT_ADDRESS);
        _write_serial_byte(data);
        _end_condition();

        _start_condition();
        _write_serial_byte(intensity);
        _end_condition();
    }
  
  public:
    void light_up(int index) {
        byte data = 0b00000001 << index;
        _led8_set(data, PULSE_WIDTH[0]);
    }
};

#endif
