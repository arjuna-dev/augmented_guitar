# Augmented Guitar - Arduino

###### _Note: Left-Hand/Right-Hand_

_The project is divided in detection of the position of the left hand in the fretboard (which will be referred to as left-hand in this repository) and strumming detection of each of the strings, (referred to as right hand)._

#### Right-Hand

To detect weather strings are vibrating or not an hexaphonic pickup is used to detect the vibration of each string. Each sine wave is then amplified and rectified before going to an input of the MCU (Teensy 3.6). **Pickup->Amplifier->Full-bridge Rectifier->MCU**

Refer to the folder Right-hand schematic to see the electronics schematic under the Schematics folder. The json file can be opened and edited using the EasyEDA software.

#### Left-Hand

To detect the position of the left hand the capacitive touch sensing feature of the Teensy 3.6 is used. As it is too time consuming to iterate through all of the touch sensing pads a non-blocking version of the capacitive feature is used with a modified version of this library https://github.com/arjuna-dev/TeensyTouch

Refer to the folder Left-hand schematic under the Schematics folder to see the electronics schematic. The json file can be opened and edited using the EasyEDA software.

#### Augmented Guitar

The Arduino code can detect pressing of frets and and strumming of notes and then sends this as MIDI over USB. The Teensy standard library is used to send MIDI messages.

As notes are repeated throughout the fretboard an additional MIDI message is sent to specify on which fret the MIDI note is being played. A CC message is used for this purpose. The CC messages 20, 21, 22 and 23 where used as they are "undefined" as per MIDI standards. This message always precedes a note_on or note_off MIDI message. A different CC message is used to specify weather the message is for a _pressed_ fret, a _lifted_ fret, a _note on_ fret or a _note off_ fret. For now, channel 1 is hardcoded.

```Arduino
  usbMIDI.sendControlChange(20, fret, 1);
  usbMIDI.sendNoteOn(note, velocity, 1);
```

```Arduino
  usbMIDI.sendControlChange(21, fret, 1);
  usbMIDI.sendNoteOff(note, 0, 1);
```

```Arduino
  usbMIDI.sendControlChange(22, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
```

```Arduino
  usbMIDI.sendControlChange(23, fret, 1);
  usbMIDI.sendNoteOn(note, 0, 1);
```
