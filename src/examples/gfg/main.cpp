#include <iostream>
#include "class.h"

using namespace std;

int main() {
  TV tv;
  Stereo stereo;

  TurnOnCommand on_tv(tv);
  TurnOffCommand off_tv(tv);
  TurnOnCommand on_stereo(stereo);
  TurnOffCommand off_stereo(stereo);

  RemoteControl remote;

  remote.set_command(on_tv);
  remote.press_button();

  remote.set_command(off_tv);
  remote.press_button();

  remote.set_command(on_stereo);
  remote.press_button();

  remote.set_command(off_stereo);
  remote.press_button();

  return 0;
}