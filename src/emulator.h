#pragma once
#include <array>
#include <string>

#include "display.h"
#include "input.h"
#include "speaker.h"

namespace c8emu {

const int CHIP8_MEMORY_SIZE = 4096;
const int CHIP8_V_REGISTER_COUNT = 16;
const int CHIP8_STACK_SIZE = 16;
const int CHIP8_FONTSET_SIZE = 80;
const int CHIP8_PROGRAM_START_LOCATION = 0x200;

struct EmulationConfig {
  std::string path_to_rom = "../roms/PONG";
  int clock_ms = 2;
};

class Chip8Emulator {
 public:
  Chip8Emulator(EmulationConfig& config, Input& input, Display& display, Speaker& speaker);
  ~Chip8Emulator() {}

  // Call this method every millisecond for the simulation to work
  void EmulateMillisecond();

  // Load a program into the memory (reseting the system)
  void LoadProgram(const std::string& path_to_rom);

  // Reset the chip to default state
  void Reset();

 private:
  void EmulateCycle();
  void FetchOperation();

  Input& input;
  Display& display;
  Speaker& speaker;
  EmulationConfig& config;

  // Operations
  void OpNull();

  void Op0___();
  void Op0NNN();
  void Op00E0();
  void Op00EE();
  void Op1NNN();
  void Op2NNN();
  void Op3XNN();
  void Op4XNN();
  void Op5XY0();
  void Op6XNN();
  void Op7XNN();

  void Op8___();
  void Op8XY0();
  void Op8XY1();
  void Op8XY2();
  void Op8XY3();
  void Op8XY4();
  void Op8XY5();
  void Op8XY6();
  void Op8XY7();
  void Op8XYE();

  void Op9XY0();
  void OpANNN();
  void OpBNNN();
  void OpCXNN();
  void OpDXYN();
  void OpE___();
  void OpEX9E();
  void OpEXA1();
  void OpF___();
  void OpFX07();
  void OpFX0A();
  void OpFX15();
  void OpFX18();
  void OpFX1E();
  void OpFX29();
  void OpFX33();
  void OpFX55();
  void OpFX65();

  void (Chip8Emulator::*Chip8MainOps[16])() = {
      &Chip8Emulator::Op0___, &Chip8Emulator::Op1NNN, &Chip8Emulator::Op2NNN,
      &Chip8Emulator::Op3XNN, &Chip8Emulator::Op4XNN, &Chip8Emulator::Op5XY0,
      &Chip8Emulator::Op6XNN, &Chip8Emulator::Op7XNN, &Chip8Emulator::Op8___,
      &Chip8Emulator::Op9XY0, &Chip8Emulator::OpANNN, &Chip8Emulator::OpBNNN,
      &Chip8Emulator::OpCXNN, &Chip8Emulator::OpDXYN, &Chip8Emulator::OpE___,
      &Chip8Emulator::OpF___};

  void (Chip8Emulator::*Chip8ArithmethicOps[16])() = {
      &Chip8Emulator::Op8XY0, &Chip8Emulator::Op8XY1, &Chip8Emulator::Op8XY2,
      &Chip8Emulator::Op8XY3, &Chip8Emulator::Op8XY4, &Chip8Emulator::Op8XY5,
      &Chip8Emulator::Op8XY6, &Chip8Emulator::Op8XY7, &Chip8Emulator::OpNull,
      &Chip8Emulator::OpNull, &Chip8Emulator::OpNull, &Chip8Emulator::OpNull,
      &Chip8Emulator::OpNull, &Chip8Emulator::OpNull, &Chip8Emulator::Op8XYE,
      &Chip8Emulator::OpNull};

  int milliseconds_counter;

  uint16_t current_operation;
  uint16_t index_register;
  uint16_t program_counter;

  uint16_t stack_pointer;

  uint8_t delay_timer;
  uint8_t sound_timer;

  std::array<uint8_t, CHIP8_MEMORY_SIZE> memory;
  std::array<uint8_t, CHIP8_V_REGISTER_COUNT> v_register;
  std::array<uint8_t, CHIP8_DISPLAY_SIZE> display_data;
  std::array<uint8_t, CHIP8_INPUT_COUNT> input_data;
  std::array<uint16_t, CHIP8_STACK_SIZE> stack;

  const std::array<uint8_t, CHIP8_FONTSET_SIZE> font_set = {
      0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
      0x20, 0x60, 0x20, 0x20, 0x70,  // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
      0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
      0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
      0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
      0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
      0xF0, 0x80, 0xF0, 0x80, 0x80   // F
  };
};

}  // namespace c8emu