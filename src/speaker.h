#pragma once
#include <SDL.h>

#include <queue>

namespace c8emu {

struct SpeakerConfig {
  uint16_t beep_frequency = 900;
  uint16_t beep_duration = 100;
};

class Speaker {
 public:
  Speaker(SpeakerConfig& config);
  ~Speaker();

  // Beep with default frequency and duration
  void Beep();

  // Beep for duration milliseconds in the given frequency
  void Beep(double frequency, int duration);

 private:
  const int playback_frequency = 44100;
  const int playback_amplitude = 20000;

  struct BeepObject {
    double frequency;
    int samples_left;
  };

  SpeakerConfig& config;

  // SDL Audio device id
  SDL_AudioDeviceID device_id;

  // Beeps waiting to be processed
  std::queue<BeepObject> beeps;

  // Position of the sample (see FillSamples method)
  double sample_position = 0;

  // Method for SDL to call
  static void FillSamples(void* object, uint8_t* input_stream, int stream_length);
};
}  // namespace c8emu
