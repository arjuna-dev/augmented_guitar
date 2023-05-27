#include "test_fixtures.h"
#include "../device_specs/device_specs.h"
#include "../MIDI/midi_methods_mock.h"
#include "../AnalogReader/analog_reader_mock.h"

GuitarStringFixture::GuitarStringFixture() {
    mock_pressed_frets_values = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    };
    midi_methods_mock = new MIDIMethodsMock();
}

void GuitarStringFixture::setup() {
    TestOnce::setup();
    // make copies of sine waves
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
        _strings_sine_wave_mocks[i] = strings_sine_wave_mocks[i];
    }
    for (int i = 0; i < NUM_OF_STRINGS; i++) {
        AnalogReaderInterface* analog_reader_mock = new AnalogReaderMock(_strings_sine_wave_mocks[i], mock_pressed_frets_values, i);
        guitar_string_mocks.push_back(GuitarString(analog_reader_mock, midi_methods_mock, i, string_input_pins_mock[i], open_string_notes_mock[i], max_amplitudes_mock[i], min_thresholds_mock[i], max_wave_periods_mock[i]));
    }
}

void GuitarStringFixture::teardown() {
    // Teardown code here
    TestOnce::teardown();
}
