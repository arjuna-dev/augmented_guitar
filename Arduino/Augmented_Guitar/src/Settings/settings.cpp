#include "settings.h"

Settings::Settings() : 
  hammer_ons_pull_ups_enabled(false),
  playing_mode(PLAYING_MODE_PROTECTED_CHORDS),
  note_off_mode(NOTE_OFF_MODE_FINGER_LIFT)
  {}

Settings settings;