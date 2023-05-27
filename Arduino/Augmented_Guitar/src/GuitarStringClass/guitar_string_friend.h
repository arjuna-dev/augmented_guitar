#ifndef GUITAR_STRING_FRIEND_CLASS_H
#define GUITAR_STRING_FRIEND_CLASS_H

#pragma once

#include "guitar_string.h"

class GuitarStringFriend {
public:
  bool get_note_on(GuitarString& obj);
  void set_note_on(GuitarString& obj, bool value);
  void set_note_on_timestamp(GuitarString& obj, unsigned long value);
  void set_current_amplitude(GuitarString& obj, int value);
};

#endif