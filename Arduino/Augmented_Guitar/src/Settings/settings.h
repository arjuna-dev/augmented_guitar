#ifndef SETTINGS_H
#define SETTINGS_H

enum PlayingMode {
  PLAYING_MODE_RAW,
  PLAYING_MODE_PROTECTED_CHORDS,
  PLAYING_MODE_LEFT_HAND_CHORDS,
  PLAYING_MODE_LEFT_NOTES_ONLY
};

enum NoteOffMode {
  NOTE_OFF_MODE_WAVEFORM,
  NOTE_OFF_MODE_FINGER_LIFT
};

class Settings {
  public:
    Settings();
    bool hammer_ons_pull_ups_enabled;
    NoteOffMode note_off_mode;
    int playing_mode;
};

extern Settings settings;

#endif