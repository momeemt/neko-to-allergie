#ifndef VARIABLE_RESISTANCE_HPP
#define VARIABLE_RESISTANCE_HPP

class VR {
  private:
    const int VR_PIN = A1;
  
  public:
    void init() {
      pinMode(VR_PIN, INPUT);
    }

    unsigned int read() {
      return analogRead(VR_PIN);
    }
};

#endif
