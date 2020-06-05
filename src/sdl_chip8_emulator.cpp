#include "sdl_chip8_emulator.h"

#include <SDL.h>

#include <fstream>
#include <iostream>

namespace c8emu {

SDL_Chip8Emulator::SDL_Chip8Emulator(EmulationConfig& config, Input& input, Display& display,
                                     Speaker& speaker)
    : config(config), Chip8Emulator(input, display, speaker) {
  Reset();
  LoadProgram(config.path_to_rom);
}

void SDL_Chip8Emulator::EmulateMillisecond() {}

void SDL_Chip8Emulator::LoadProgram(const std::string& path_to_rom) {
  std::ifstream rom(path_to_rom, std::ios_base::binary);

  // read rom
  if (!(rom.read(reinterpret_cast<char*>(memory.data()), memory.size()))) {
    if (!rom.eof()) {
      std::cout << "An error happened while reading your rom file." << std::endl;
    }
  }
}
void SDL_Chip8Emulator::Reset() {
  current_operation = 0x00;

  index_register = 0;
  program_counter = 0;
  stack_pointer = 0;

  delay_timer = 0;
  sound_timer = 0;

  memory.fill(0);
  v_register.fill(0);
  stack.fill(0);
  display_data.fill(0);
  input_data.fill(0);
}

}  // namespace c8emu