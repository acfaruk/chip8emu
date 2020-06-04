#pragma once
#include "chip8_emulator.h"

namespace c8emu {
class SDL_Chip8Emulator : public Chip8Emulator {
 public:
  SDL_Chip8Emulator(Input& input, Display& display, Speaker& speaker, EmulationConfig& config)
      : Chip8Emulator(input, display, speaker), config(config) {
    LoadProgram(config.path_to_rom);
  }

  // Inherited via Chip8Emulator
  virtual void EmulateCycle() override;
  virtual void LoadProgram(const std::string& path_to_rom) override;
  virtual void Reset() override;

 private:
  EmulationConfig config;

  uint16_t current_operation;

  uint8_t index_register;
  uint8_t program_counter;
  uint8_t stack_pointer;

  uint8_t delay_timer;
  uint8_t sound_timer;

  std::array<uint8_t, CHIP8_MEMORY_SIZE> memory;
  std::array<uint8_t, CHIP8_V_REGISTER_COUNT> v_register;
  std::array<uint8_t, CHIP8_STACK_SIZE> stack;
  std::array<uint8_t, CHIP8_DISPLAY_SIZE> display_data;
  std::array<uint8_t, CHIP8_INPUT_COUNT> input_data;
};

}  // namespace c8emu