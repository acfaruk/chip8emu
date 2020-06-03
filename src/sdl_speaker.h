#pragma once
#include <SDL.h>

#include <queue>

#include "speaker.h"

namespace c8emu {
class SDL_Speaker : public Speaker {
 public:
  SDL_Speaker();
  ~SDL_Speaker();

  // Inherited via Speaker
  virtual void Beep(double frequency, int duration) override;

 private:
  const int playback_frequency = 44100;
  const int playback_amplitude = 28000;

  struct BeepObject {
    double frequency;
    int samples_left;
  };

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