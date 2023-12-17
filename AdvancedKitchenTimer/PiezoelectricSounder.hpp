#ifndef PIEZOELECTRIC_SOUNDER_HPP
#define PIEZOELECTRIC_SOUNDER_HPP

struct note {
  unsigned int frequency;
  unsigned long duration;
};

class PiezoelectricSounder {
  private:
    const int PIEZO_PIN = 2;
    struct note *_melody;
    int _melodySize = 0;
    int _melodyCapacity = 10;

  public:
    PiezoelectricSounder() {
      _melody = new note[_melodyCapacity];
    }

    ~PiezoelectricSounder() {
      delete[] _melody;
    }

    void add_note(unsigned int frequency, unsigned long duration) {
      if (_melodySize >= _melodyCapacity) {
        _melodyCapacity *= 2;
        note *newMelody = new note[_melodyCapacity];
        memcpy(newMelody, _melody, sizeof(note) * _melodySize);
        delete[] _melody;
        _melody = newMelody;
      }

      struct note new_note = { frequency, duration };
      _melody[_melodySize++] = new_note;
    }

    void clear() {
      _melodySize = 0;
    }

    void play() {
      for (int thisNote = 0; thisNote < _melodySize; thisNote++) {
        unsigned int freq = _melody[thisNote].frequency;
        if (freq != NULL) {
          tone(PIEZO_PIN, freq);
        }
        delay(_melody[thisNote].duration);
        noTone(PIEZO_PIN);
      }
      noTone(PIEZO_PIN);
    }

    void init() {
      pinMode(PIEZO_PIN, OUTPUT);
    }
};

#endif
