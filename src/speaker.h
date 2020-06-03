#pragma once

namespace c8emu {
class Speaker {
 public:
  virtual ~Speaker() {}

  // Beep for duration milliseconds in the given frequency
  virtual void Beep(double frequency, int duration) = 0;
};
}  // namespace c8emu
