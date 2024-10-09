#include <iostream>

using namespace std;

/**
 * * The Command interface declares a method for executing a command.
 */
class Command {
 public:
  virtual ~Command() {}
  virtual void Execute() const = 0;
};

/**
 * The Receiver classes contain some important business logic. They know how to
 * perform all kinds of operations, associated with carrying out a request. In
 * fact, any class may serve as a Receiver.
 */
class Receiver {
 public:
  void DoSomething(const string &a) {
    cout << "Receiver: Working on (" << a << ".)\n";
  }
  void DoSomethingElse(const string &b) {
    cout << "Receiver: Also working on (" << b << ".)\n";
  }
};

/**
 * The Invoker is associated with one or several commands. It sends a request to
 * the command.
 */
class Invoker {
  /**
   * @var Command
   */
 private:
  Command *on_start_;
  /**
   * @var Command
   */
  Command *on_finish_;
  /**
   * Initialize commands.
   */
 public:
  ~Invoker() {
    delete on_start_;
    delete on_finish_;
  }

  void SetOnStart(Command *command) { this->on_start_ = command; }
  void SetOnFinish(Command *command) { this->on_finish_ = command; }
  /**
   * The Invoker does not depend on concrete command or receiver classes. The
   * Invoker passes a request to a receiver indirectly, by executing a command.
   */
  void DoSomethingImportant() {
    cout << "Invoker: Does anybody want something done before I begin?\n";
    if (this->on_start_) {
      this->on_start_->Execute();
    }
    cout << "Invoker: ...doing something really important...\n";
    cout << "Invoker: Does anybody want something done after I finish?\n";
    if (this->on_finish_) {
      this->on_finish_->Execute();
    }
  }
};
/**
 * The client code can parameterize an invoker with any commands.
 */

/**
 * * Some commands can implement simple operations on their own.
 */
class SimpleCommand : public Command {
 private:
  string pay_load_;

 public:
  /**
   * * 'explicit' keyword is used to avoid implicit conversion of constructor.
   * For example:
   * SimpleCommand command = "Hello";                 // This will throw error.
   * SimpleCommand command("Hello");                  // This is correct.
   * SimpleCommand command = SimpleCommand("Hello");  // This is correct.
   */
  explicit SimpleCommand(string pay_load) : pay_load_(pay_load) {}

  void Execute() const override {
    cout << "SimpleCommand: See, I can do simple things like printing ("
         << this->pay_load_ << ")\n";
  }
};

/**
 * However, some commands can delegate more complex operations to other objects,
 * called "receivers."
 */
class ComplexCommand : public Command {
  /**
   * @var Receiver
   */
 private:
  Receiver *receiver_;
  /**
   * Context data, required for launching the receiver's methods.
   */
  string a_;
  string b_;
  /**
   * Complex commands can accept one or several receiver objects along with any
   * context data via the constructor.
   */
 public:
  ComplexCommand(Receiver *receiver, string a, string b)
      : receiver_(receiver), a_(a), b_(b) {}
  /**
   * Commands can delegate to any methods of a receiver.
   */
  void Execute() const override {
    cout << "ComplexCommand: Complex stuff should be done by a receiver "
            "object.\n";
    this->receiver_->DoSomething(this->a_);
    this->receiver_->DoSomethingElse(this->b_);
  }
};

int main() {
  Invoker *invoker = new Invoker;
  invoker->SetOnStart(new SimpleCommand("Say Hi!"));

  Receiver *receiver = new Receiver;
  invoker->SetOnFinish(
      new ComplexCommand(receiver, "Send email", "Save report"));
  invoker->DoSomethingImportant();

  delete invoker;
  delete receiver;

  return 0;
}