#ifndef TESTS_H
#define TESTS_H

#include "AUnit.h"

test(correct) {
  int x = 1;
  assertEqual(x, 1);
}

test(incorrect) {
  int x = 1;
  assertNotEqual(x, 1);
}

#endif