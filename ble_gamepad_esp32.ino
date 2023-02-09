#include <BleGamepad.h>

BleGamepad bleGamepad;

#define npins 10

char pins[npins] =   { 14, 17, 18, 21, 22, 26, 32, 33, 19, 16 };
char btnmap[npins] = {  6,  4,  5,  1,  10,  2,  9,  3,  8,  7 };
char states[npins] = { 0 };

// floating 23

// 22 32
// 33 17 14 18
// 21 26 19 16

//    27
// 13    4
//    25

void setup()
{
  BleGamepadConfiguration bleGamepadConfig;
  // bleGamepadConfig.setButtonCount(10);
  // bleGamepadConfig.setHatSwitchCount(1);
  // bleGamepadConfig.setWhichAxes(false, false, false, false, false, false, false, false);

  bleGamepad.begin(&bleGamepadConfig);

  for (int i = 0; i < npins; i++) pinMode(pins[i], INPUT_PULLUP);

  pinMode(13, INPUT_PULLUP); // left
  pinMode(27, INPUT_PULLUP); // up
  pinMode(25, INPUT_PULLUP); // down
  pinMode( 4, INPUT_PULLUP); // right
}

int dpad = 0;

void loop()
{
  int i, old, changed = 0;

  if(bleGamepad.isConnected()) {
    for (i = 0; i < npins; i++) {
      old = states[i];
      states[i] = digitalRead(pins[i]);
      if (old != states[i]) {
        changed = 1;
        states[i] ? bleGamepad.release(btnmap[i]) : bleGamepad.press(btnmap[i]);
      }
    }

    old = dpad;
    int left = !digitalRead(13);
    int right = !digitalRead(4); 
    if (!digitalRead(27)) { // up
      if (left) {
        dpad = DPAD_UP_LEFT;
      } else if (right) {
        dpad = DPAD_UP_RIGHT;
      } else {
        dpad = DPAD_UP;
      }
    } else if (!digitalRead(25)) { // down
      if (left) {
        dpad = DPAD_DOWN_LEFT;
      } else if (right) {
        dpad = DPAD_DOWN_RIGHT;
      } else {
        dpad = DPAD_DOWN;
      }
    } else if (left) {
      dpad = DPAD_LEFT;
    } else if (right) {
      dpad = DPAD_RIGHT;
    } else {
      dpad = DPAD_CENTERED;
    }

    if (dpad != old) {
      changed = 1;
      bleGamepad.setHat(dpad);
    }

    if (changed) bleGamepad.sendReport();
  }

  delay(1);
}
