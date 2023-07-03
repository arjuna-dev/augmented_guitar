#include "settings.h"

Settings::Settings() : 
  playing_mode(PLAYING_MODE_NOTES_ONLY),
  hammer_ons_pull_ups_enabled(false),
  note_off_mode(NOTE_OFF_MODE_WAVEFORM)
  {}

Settings settings;