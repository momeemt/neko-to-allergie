#ifndef SEVEN_SEGMENT_LED_HPP
#define SEVEN_SEGMENT_LED_HPP

class SevenSegmentLED {
  private:
    const int SEG_G = 3;
    const int SEG_F = 4;
    const int SEG_E = 5;
    const int SEG_D = 6;
    const int SEG_C = 7;
    const int SEG_B = 8;
    const int SEG_A = 9;
    const int SEG7_BIN0 = 10;
    const int SEG7_BIN1 = 11;

    void _write_a_digit(byte digit, byte data) {
      const byte SEG7_DIG0[] = {1, 0, 1, 0};
      const byte SEG7_DIG1[] = {1, 1, 0, 0};

      digitalWrite(SEG7_BIN0, SEG7_DIG0[digit]);
      digitalWrite(SEG7_BIN1, SEG7_DIG1[digit]);

      const byte SEG_GP[] = {0, 0, 1, 1, 1, 1, 1, 0, 1, 1};
      const byte SEG_FP[] = {1, 0, 0, 0, 1, 1, 1, 1, 1, 1};
      const byte SEG_EP[] = {1, 0, 1, 0, 0, 0, 1, 0, 1, 0};
      const byte SEG_DP[] = {1, 0, 1, 1, 0, 1, 1, 0, 1, 1};
      const byte SEG_CP[] = {1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
      const byte SEG_BP[] = {1, 1, 1, 1, 1, 0, 0, 1, 1, 1};
      const byte SEG_AP[] = {1, 0, 1, 1, 0, 1, 1, 1, 1, 1};
      
      if (data < 10) {
        digitalWrite(SEG_G, SEG_GP[data]);
        digitalWrite(SEG_F, SEG_FP[data]);
        digitalWrite(SEG_E, SEG_EP[data]);
        digitalWrite(SEG_D, SEG_DP[data]);
        digitalWrite(SEG_C, SEG_CP[data]);
        digitalWrite(SEG_B, SEG_BP[data]);
        digitalWrite(SEG_A, SEG_AP[data]);
      } else {
        digitalWrite(SEG_G, 1);
        digitalWrite(SEG_F, 0);
        digitalWrite(SEG_E, 0);
        digitalWrite(SEG_D, 1);
        digitalWrite(SEG_C, 0);
        digitalWrite(SEG_B, 0);
        digitalWrite(SEG_A, 1);
      }
    }

    void _clear_7seg() {
      for (byte i = SEG_G; i <= SEG_A; i++) {
        digitalWrite(i, 0);
      }
    }
  
  public:
    void write(int data) {
      byte digit[] = {0, 0, 0, 0};
      for (byte i = 0; i < 4; i++) {
        digit[i] = data % 10;
        data /= 10;
      }
      for (byte i = 0; i < 4; i++) {
        _write_a_digit(i, digit[i]);
        delay(1);
        _clear_7seg();
      }
    }

    void write(byte digit, byte data) {
      _write_a_digit(digit, data);
      delay(1);
      _clear_7seg();
    }

    void write(byte data[4]) {
      for (byte i = 0; i < 4; i++) {
        _write_a_digit(i, data[i]);
        delay(1);
        _clear_7seg();
      }
    }

    void init() {
      for (byte i = SEG_G; i <= SEG7_BIN1; i++) {
        ::pinMode(i, OUTPUT);
      }
    }
};

#endif
