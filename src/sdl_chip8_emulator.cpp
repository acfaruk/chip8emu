#include "sdl_chip8_emulator.h"

#include <SDL.h>

#include <algorithm>
#include <fstream>
#include <iostream>

namespace c8emu {

SDL_Chip8Emulator::SDL_Chip8Emulator(EmulationConfig& config, Input& input, Display& display,
                                     Speaker& speaker)
    : config(config), Chip8Emulator(input, display, speaker) {
  Reset();
  LoadProgram(config.path_to_rom);
}

void SDL_Chip8Emulator::EmulateMillisecond() {
  if (milliseconds_counter % config.clock_ms == 0) {
    EmulateCycle();
  }

  if (milliseconds_counter % 17 == 0) {  // around 60Hz
    display.SetScreen(display_data);
    input.SetInput(input_data);

    if (delay_timer > 0) delay_timer--;
    if (sound_timer > 0) {
      if (sound_timer == 1) {
        speaker.Beep();
      }
      sound_timer--;
    }
  }

  milliseconds_counter++;
}

void SDL_Chip8Emulator::LoadProgram(const std::string& path_to_rom) {
  std::ifstream rom(path_to_rom, std::ios_base::binary);

  // read rom
  if (!(rom.read(reinterpret_cast<char*>(memory.data() + CHIP8_PROGRAM_START_LOCATION),
                 memory.size() - CHIP8_PROGRAM_START_LOCATION))) {
    if (!rom.eof()) {
      std::cout << "An error happened while reading your rom file." << std::endl;
    }
  }
}
void SDL_Chip8Emulator::Reset() {
  program_counter = CHIP8_PROGRAM_START_LOCATION;

  milliseconds_counter = 0;

  current_operation = 0;
  index_register = 0;
  stack_pointer = 0;

  delay_timer = 0;
  sound_timer = 0;

  memory.fill(0);
  v_register.fill(0);
  stack.fill(0);
  display_data.fill(0);
  input_data.fill(0);

  std::copy_n(font_set.begin(), font_set.size(), memory.begin());
}

void SDL_Chip8Emulator::EmulateCycle() {
  FetchOperation();

  // Run Current Operation
  (this->*(Chip8MainOps[(current_operation & 0xF000) >> 12]))();
}

void SDL_Chip8Emulator::FetchOperation() {
  current_operation = (memory[program_counter] << 8) + memory[program_counter + 1];
  // std::cout << std::hex << current_operation << std::endl;
}

void SDL_Chip8Emulator::OpNull() {}

void SDL_Chip8Emulator::Op0___() {
  switch (current_operation) {
    case 0x00E0:
      return Op00E0();
    case 0x00EE:
      return Op00EE();
    default:
      return OpNull();
  }
}

void SDL_Chip8Emulator::Op0NNN() {}

void SDL_Chip8Emulator::Op00E0() {
  display_data.fill(0);

  program_counter += 2;
}

void SDL_Chip8Emulator::Op00EE() {
  stack_pointer--;
  program_counter = stack[stack_pointer];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op1NNN() {
  uint16_t nnn = current_operation & 0x0FFF;
  program_counter = nnn;
}

void SDL_Chip8Emulator::Op2NNN() {
  uint16_t nnn = current_operation & 0x0FFF;

  stack[stack_pointer] = program_counter;
  stack_pointer++;

  program_counter = nnn;
}

void SDL_Chip8Emulator::Op3XNN() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t nn = (current_operation & 0x00FF);

  if (v_register[x] == nn) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::Op4XNN() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t nn = (current_operation & 0x00FF);

  if (v_register[x] != nn) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::Op5XY0() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;

  if (v_register[x] == v_register[y]) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::Op6XNN() {
  uint8_t nn = (current_operation & 0x00FF);
  uint8_t x = (current_operation & 0x0F00) >> 8;
  v_register[x] = nn;

  program_counter += 2;
}

void SDL_Chip8Emulator::Op7XNN() {
  uint8_t nn = (current_operation & 0x00FF);
  uint8_t x = (current_operation & 0x0F00) >> 8;

  v_register[x] += nn;

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8___() {
  // Run arithmetic operation
  (this->*(Chip8ArithmethicOps[(current_operation & 0x000F)]))();
}

void SDL_Chip8Emulator::Op8XY0() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[x] = v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY1() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[x] = v_register[x] | v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY2() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[x] = v_register[x] & v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY3() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[x] = v_register[x] ^ v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY4() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;

  if (v_register[y] > (0xFF - v_register[x])) {
    v_register[0xF] = 1;
  } else {
    v_register[0xF] = 0;
  }
  v_register[x] += v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY5() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;

  if (v_register[y] > v_register[x]) {
    v_register[0xF] = 0;
  } else {
    v_register[0xF] = 1;
  }
  v_register[x] -= v_register[y];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY6() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[0xF] = v_register[x] & 0x1;
  v_register[x] >>= 1;

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XY7() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;

  if (v_register[x] > v_register[y]) {
    v_register[0xF] = 0;
  } else {
    v_register[0xF] = 1;
  }
  v_register[x] = v_register[y] - v_register[x];

  program_counter += 2;
}

void SDL_Chip8Emulator::Op8XYE() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  v_register[0xF] = v_register[x] >> 7;
  v_register[x] <<= 1;

  program_counter += 2;
}

void SDL_Chip8Emulator::Op9XY0() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;

  if (v_register[x] != v_register[y]) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::OpANNN() {
  uint16_t nnn = current_operation & 0x0FFF;
  index_register = nnn;
  program_counter += 2;
}

void SDL_Chip8Emulator::OpBNNN() {
  uint16_t nnn = current_operation & 0x0FFF;
  program_counter = v_register[0] + nnn;
}

void SDL_Chip8Emulator::OpCXNN() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t nn = (current_operation & 0x00FF);

  v_register[x] = (std::rand() % 255) & nn;

  program_counter += 2;
}

void SDL_Chip8Emulator::OpDXYN() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  uint8_t y = (current_operation & 0x00F0) >> 4;
  uint8_t n = (current_operation & 0x000F);

  uint8_t v_x = v_register[x];
  uint8_t v_y = v_register[y];

  v_register[0xF] = 0;

  for (int y_line = 0; y_line < n; y_line++) {
    uint8_t pixels = memory[index_register + y_line];

    for (int x_line = 0; x_line < 8; x_line++) {
      if ((pixels & (0x80 >> x_line)) != 0) {
        uint16_t pixel_index =
            v_register[x] + x_line + ((v_register[y] + y_line) * CHIP8_DISPLAY_WIDTH);

        if (display_data[pixel_index] == 1) {
          v_register[0xF] = 1;
        }
        display_data[pixel_index] ^= 1;
      }
    }
  }

  program_counter += 2;
}

void SDL_Chip8Emulator::OpE___() {
  switch (current_operation & 0x00FF) {
    case 0x9E:
      return OpEX9E();
    case 0xA1:
      return OpEXA1();
    default:
      return OpNull();
  }
}

void SDL_Chip8Emulator::OpEX9E() {
  uint8_t x = (current_operation & 0x0F00) >> 8;

  if (input_data[v_register[x]]) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::OpEXA1() {
  uint8_t x = (current_operation & 0x0F00) >> 8;

  if (!input_data[v_register[x]]) {
    program_counter += 4;
  } else {
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::OpF___() {
  switch (current_operation & 0x00FF) {
    case 0x07:
      return OpFX07();
    case 0x0A:
      return OpFX0A();
    case 0x15:
      return OpFX15();
    case 0x18:
      return OpFX18();
    case 0x1E:
      return OpFX1E();
    case 0x29:
      return OpFX29();
    case 0x33:
      return OpFX33();
    case 0x55:
      return OpFX55();
    case 0x65:
      return OpFX65();
    default:
      return OpNull();
  }
}

void SDL_Chip8Emulator::OpFX07() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  v_register[x] = delay_timer;

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX0A() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  auto key = std::find(input_data.begin(), input_data.end(), 1);
  if (key != input_data.end()) {
    v_register[x] = *key;
    program_counter += 2;
  }
}

void SDL_Chip8Emulator::OpFX15() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  delay_timer = v_register[x];

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX18() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  sound_timer = v_register[x];

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX1E() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  if (index_register + v_register[x] > 0xFFF) {
    v_register[0xF] = 1;
  } else {
    v_register[0xF] = 0;
  }
  index_register += v_register[x];

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX29() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  index_register = x * 0x5;

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX33() {
  uint8_t x = (current_operation & 0x0F00) >> 8;
  memory[index_register] = v_register[x] / 100;
  memory[index_register + 1] = (v_register[x] / 10) % 10;
  memory[index_register + 2] = (v_register[x] % 100) % 10;

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX55() {
  uint8_t x = (current_operation & 0x0F00) >> 8;

  std::copy_n(v_register.begin(), x + 1, memory.begin() + index_register);

  // Emulate original CHIP8 behaviour
  index_register += (x + 1);

  program_counter += 2;
}

void SDL_Chip8Emulator::OpFX65() {
  uint8_t x = (current_operation & 0x0F00) >> 8;

  std::copy_n(memory.begin() + index_register, x + 1, v_register.begin());

  // Emulate original CHIP8 behaviour
  index_register += (x + 1);

  program_counter += 2;
}

}  // namespace c8emu