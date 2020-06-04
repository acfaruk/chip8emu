#include "sdl_chip8_emulator.h"

#include <fstream>
#include <iostream>

namespace c8emu {
void SDL_Chip8Emulator::EmulateCycle() { speaker.Beep(); }
void SDL_Chip8Emulator::LoadProgram(const std::string& path_to_rom) {
  std::ifstream rom(path_to_rom, std::ios_base::binary);

  uint8_t buffer[CHIP8_MEMORY_SIZE];

  // read file
  if (!(rom.read((char*)buffer, sizeof(buffer)))) {
    if (!rom.eof()) {
      std::cout << "An error happened while reading your rom file." << std::endl;
    }
  }
  size_t bytes_read = rom.gcount();
}
void SDL_Chip8Emulator::Reset() {}
}  // namespace c8emu