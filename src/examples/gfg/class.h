#pragma once

#include <iostream>

/*
 * Command interface
 */
class Command {
 public:
  virtual ~Command() {}

  /*
   * `= 0`: This syntax makes the function a pure virtual function. A pure
   * virtual function does not have an implementation in the base class and must
   * be overridden in any derived class. Declaring a pure virtual function makes
   * the containing class abstract, meaning you cannot instantiate objects of
   * that class directly.
   */
  virtual void execute() const = 0;
};

/*
 * Receiver interface
 */
class Device {
 public:
  virtual void on() const = 0;
  virtual void off() const = 0;
};

/*
 * Invoker
 */
class RemoteControl {
 private:
  Command *command;

 public:
  void set_command(Command &cmd);
  void press_button();
};

class TurnOnCommand : public Command {
 private:
  Device &device_;

 public:
  explicit TurnOnCommand(Device &device) : device_(device) {}

  void execute() const override;
};

class TurnOffCommand : public Command {
 private:
  Device &device_;

 public:
  explicit TurnOffCommand(Device &device) : device_(device) {}

  void execute() const override;
};

class TV : public Device {
 public:
  void on() const override;
  void off() const override;
  void change_channel() const;
};

class Stereo : public Device {
 public:
  void on() const override;
  void off() const override;
  void set_volume() const;
};