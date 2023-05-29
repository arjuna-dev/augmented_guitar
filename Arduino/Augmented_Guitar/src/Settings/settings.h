#ifndef SETTINGS_H
#define SETTINGS_H

enum PlayingMode {
  PLAYING_MODE_RAW,
  PLAYING_MODE_PROTECTED_CHORDS,
  PLAYING_MODE_LEFT_HAND_CHORDS,
  PLAYING_MODE_LEFT_NOTES_ONLY
};

class Settings {
  public:
    Settings();
  private:
    int _playing_mode;
};

#endif