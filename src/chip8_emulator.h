#pragma once
#include "display.h"
#include "input.h"
#include "speaker.h"

namespace c8emu {

const int CHIP8_MEMORY_SIZE = 4096;
const int CHIP8_V_REGISTER_COUNT = 16;
const int CHIP8_STACK_SIZE = 16;

class Chip8Emulator {
 public:
  Chip8Emulator(Input& input, Display& display, Speaker& speaker)
      : input(input), display(display), speaker(speaker){};
  virtual ~Chip8Emulator() {}

  // Call this method every millisecond for the simulation to work
  virtual void EmulateMillisecond() = 0;

  // Load a program into the memory (reseting the system)
  virtual void LoadProgram(const std::string& path_to_rom) = 0;

  // Reset the chip to default state
  virtual void Reset() = 0;

 protected:
  Input& input;
  Display& display;
  Speaker& speaker;
};
}  // namespace c8emu