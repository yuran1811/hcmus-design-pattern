#include "class.h"

using namespace std;

void TurnOnCommand::execute() const { device_.on(); }

void TurnOffCommand::execute() const { device_.off(); }

void TV::on() const { cout << "TV is on\n"; }
void TV::off() const { cout << "TV is off\n"; }
void TV::change_channel() const { cout << "TV channel was changed!\n"; }

void Stereo::on() const { cout << "Stereo is on\n"; }
void Stereo::off() const { cout << "Stereo is off\n"; }
void Stereo::set_volume() const { cout << "Stereo volume was set!\n"; }

void RemoteControl::set_command(Command &cmd) { command = &cmd; }
void RemoteControl::press_button() { command->execute(); }