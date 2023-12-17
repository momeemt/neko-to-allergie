#include "pitches.h"
#include "../utils/PiezoelectricSounder.hpp";

void construct_melody(PiezoelectricSounder *ps) {
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_F4, 500);
  ps->add_note(NOTE_F5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_F5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_F4, 500);
  ps->add_note(NOTE_F5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_F5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_F4, 500);
  ps->add_note(NOTE_F5, 500);
  ps->add_note(NOTE_D4, 500);
  ps->add_note(NOTE_F5, 500);
  ps->add_note(NOTE_C4, 500);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_C4, 500);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_C4, 500);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_E5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_D5, 250);
  ps->add_note(NOTE_C5, 250);
  ps->add_note(NOTE_C4, 500);
  ps->add_note(NOTE_E5, 500);
  ps->add_note(NOTE_D4, 500);
  ps->add_note(NOTE_E5, 500);
  ps->add_note(NOTE_F4, 500);
  ps->add_note(NOTE_F5, 250);
  ps->add_note(NULL, 250);
  ps->add_note(NOTE_F5, 250); 
}

void setup() {
  auto ps = PiezoelectricSounder();
  ps.init();
  construct_melody(&ps);
  ps.play();
}

void loop() {}
