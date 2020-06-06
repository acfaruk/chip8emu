#pragma once
#include "chip8_emulator.h"

namespace c8emu {
class SDL_Chip8Emulator : public Chip8Emulator {
 public:
  SDL_Chip8Emulator(EmulationConfig& config, Input& input, Display& display, Speaker& speaker);

  // Inherited via Chip8Emulator
  virtual void EmulateMillisecond() override;
  virtual void LoadProgram(const std::string& path_to_rom) override;
  virtual void Reset() override;

 private:
  void EmulateCycle();
  void FetchOperation();

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

  void (SDL_Chip8Emulator::*Chip8MainOps[16])() = {
      &SDL_Chip8Emulator::Op0___, &SDL_Chip8Emulator::Op1NNN, &SDL_Chip8Emulator::Op2NNN,
      &SDL_Chip8Emulator::Op3XNN, &SDL_Chip8Emulator::Op4XNN, &SDL_Chip8Emulator::Op5XY0,
      &SDL_Chip8Emulator::Op6XNN, &SDL_Chip8Emulator::Op7XNN, &SDL_Chip8Emulator::Op8___,
      &SDL_Chip8Emulator::Op9XY0, &SDL_Chip8Emulator::OpANNN, &SDL_Chip8Emulator::OpBNNN,
      &SDL_Chip8Emulator::OpCXNN, &SDL_Chip8Emulator::OpDXYN, &SDL_Chip8Emulator::OpE___,
      &SDL_Chip8Emulator::OpF___};

  void (SDL_Chip8Emulator::*Chip8ArithmethicOps[16])() = {
      &SDL_Chip8Emulator::Op8XY0, &SDL_Chip8Emulator::Op8XY1, &SDL_Chip8Emulator::Op8XY2,
      &SDL_Chip8Emulator::Op8XY3, &SDL_Chip8Emulator::Op8XY4, &SDL_Chip8Emulator::Op8XY5,
      &SDL_Chip8Emulator::Op8XY6, &SDL_Chip8Emulator::Op8XY7, &SDL_Chip8Emulator::OpNull,
      &SDL_Chip8Emulator::OpNull, &SDL_Chip8Emulator::OpNull, &SDL_Chip8Emulator::OpNull,
      &SDL_Chip8Emulator::OpNull, &SDL_Chip8Emulator::OpNull, &SDL_Chip8Emulator::Op8XYE,
      &SDL_Chip8Emulator::OpNull};

  EmulationConfig config;
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