#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @enum PlayingMode
 * 
 * Enum representing different modes of playing music.
 * 
 * @var PLAYING_MODE_RAW
 * MIDI messages are sent as detected irrespective of them being chords or notes.
 * 
 * @var PLAYING_MODE_CHORDS_ONLY
 * All the notes of the chord will keep playing until all the frets are unpressed.
 * 
 * @var PLAYING_MODE_NOTES_ONLY
 * Only one note can be played at a time.
 * 
 * @var PLAYING_MODE_CHORD_NOTE_AUTO
 * If one fret is pressed, we enter notes only mode. If more than one fret is pressed we enter chords only mode.
 * 
 * @var PLAYING_MODE_INFINITE_SUSTAIN_CHORDS
 * When 2 or more frets are pressed the 6 strings are played. (or 5 for Am, etc.)
 * 
 * @var PLAYING_MODE_INFINITE_SUSTAIN_NOTES
 * When a fret is pressed, the note is played until the fret is unpressed.
 */

enum PlayingMode {
  PLAYING_MODE_RAW,
  PLAYING_MODE_CHORDS_ONLY,
  PLAYING_MODE_NOTES_ONLY,
  PLAYING_MODE_CHORD_NOTE_AUTO
};

enum SustainMode {
  SUSTAIN_MODE_INFINITE,
  SUSTAIN_MODE_SHORT,
  SUSTAIN_MODE_MEDIUM,
  SUSTAIN_MODE_LONG,
  SUSTAIN_MODE_WAVEFORM,
};

class Settings {
  public:
    Settings();
    PlayingMode playing_mode;
    SustainMode sustain_mode;
    bool hammer_ons_pull_ups_enabled;
};

extern Settings settings;

#endif