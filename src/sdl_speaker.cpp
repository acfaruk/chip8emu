
#include "sdl_speaker.h"

#include <SDL_audio.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace c8emu {
SDL_Speaker::SDL_Speaker() {
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
  }

  SDL_AudioSpec desired_spec;
  desired_spec.freq = playback_frequency;
  desired_spec.format = AUDIO_S16SYS;
  desired_spec.channels = 1;
  desired_spec.samples = 4096;
  desired_spec.callback = FillSamples;
  desired_spec.userdata = this;

  device_id = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);

  if (device_id == 0) {
    throw std::runtime_error("SDL opening audio device failed: " + std::string(SDL_GetError()));
  }

  SDL_PauseAudioDevice(device_id, 0);
}

SDL_Speaker::~SDL_Speaker() { SDL_CloseAudioDevice(device_id); }

void SDL_Speaker::Beep(double frequency, int duration) {
  BeepObject new_beep;
  new_beep.frequency = frequency;
  new_beep.samples_left = duration * playback_frequency / 1000;

  SDL_LockAudioDevice(device_id);
  beeps.push(new_beep);
  SDL_UnlockAudioDevice(device_id);
}

void SDL_Speaker::FillSamples(void *object, uint8_t *stream, int stream_length) {
  int length = stream_length / 2;
  int16_t *audio_stream = (int16_t *)stream;
  SDL_Speaker *speaker = (SDL_Speaker *)object;

  int i = 0;
  while (i < length) {
    if (speaker->beeps.empty()) {
      while (i < length) {
        audio_stream[i] = 0;
        i++;
      }
      return;
    }

    BeepObject &current_beep = speaker->beeps.front();
    int samples_to_do = std::min(i + current_beep.samples_left, length);
    current_beep.samples_left -= samples_to_do - i;

    while (i < samples_to_do) {
      int a = speaker->playback_amplitude;
      int f = speaker->playback_frequency;
      double pos = speaker->sample_position;

      audio_stream[i] = (int16_t)(a * std::sin(pos * 2 * M_PI / f));
      i++;
      speaker->sample_position += current_beep.frequency;
    }

    if (current_beep.samples_left == 0) {
      speaker->beeps.pop();
    }
  }
}
}  // namespace c8emu