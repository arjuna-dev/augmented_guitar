#include "settings.h"

Settings::Settings() : 
  playing_mode(PLAYING_MODE_CHORDS_ONLY),
  sustain_mode(SUSTAIN_MODE_WAVEFORM),
  hammer_ons_pull_ups_enabled(false)
  {}

Settings settings;