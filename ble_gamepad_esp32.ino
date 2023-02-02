#include <Arduino.h>
#include <BleGamepad.h>

BleGamepad bleGamepad;

#define npins 10

char pins[npins] = { 4, 13, 14, 16, 17, 18, 19, 21, 22, 23 };
char states[npins] = { 0 };

void setup()
{
  int i;

  bleGamepad.begin();

  for (i = 0; i < npins; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }

  /*
  pinMode(25, INPUT_PULLUP); // up
  pinMode(26, INPUT_PULLUP); // down
  pinMode(27, INPUT_PULLUP); // left
  pinMode(32, INPUT_PULLUP); // right
  */

  // pinMode(33, INPUT_PULLUP);
}

void loop()
{
  int i, old, changed = 0;

  if(bleGamepad.isConnected()) {
    for (i = 0; i < npins; i++) {
      old = states[i];
      states[i] = digitalRead(pins[i]);
      if (old != states[i]) {
        changed = 1;
	states[i] ? bleGamepad.release(i + 1) : bleGamepad.press(i + 1);
      }
    }

    if (changed) bleGamepad.sendReport();
  }

  delay(1);
}
