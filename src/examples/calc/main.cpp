#include <iostream>
#include <memory>
#include <vector>

class Command {
 public:
  virtual ~Command() = default;

  virtual void execute() = 0;
  // virtual void undo() = 0;
  // virtual void redo() = 0;
};

// Receiver Class (Calculator)
class Calculator {
 private:
  double result;

 public:
  Calculator() : result(0.0) {}

  void add(double value) {
    result += value;
    std::cout << "Adding: " << value << ", Result: " << result << std::endl;
  }

  void subtract(double value) {
    result -= value;
    std::cout << "Subtracting: " << value << ", Result: " << result
              << std::endl;
  }

  void multiply(double value) {
    result *= value;
    std::cout << "Multiplying by: " << value << ", Result: " << result
              << std::endl;
  }

  void divide(double value) {
    if (value != 0) {
      result /= value;
      std::cout << "Dividing by: " << value << ", Result: " << result
                << std::endl;
    } else {
      std::cout << "Error: Division by zero!" << std::endl;
    }
  }

  double getResult() const { return result; }
};

class AddCommand : public Command {
 private:
  Calculator& calculator;
  double value;

 public:
  AddCommand(Calculator& calc, double val) : calculator(calc), value(val) {}

  void execute() override { calculator.add(value); }
};

class SubtractCommand : public Command {
 private:
  Calculator& calculator;
  double value;

 public:
  SubtractCommand(Calculator& calc, double val)
      : calculator(calc), value(val) {}

  void execute() override { calculator.subtract(value); }
};

class MultiplyCommand : public Command {
 private:
  Calculator& calculator;
  double value;

 public:
  MultiplyCommand(Calculator& calc, double val)
      : calculator(calc), value(val) {}

  void execute() override { calculator.multiply(value); }
};

class DivideCommand : public Command {
 private:
  Calculator& calculator;
  double value;

 public:
  DivideCommand(Calculator& calc, double val) : calculator(calc), value(val) {}

  void execute() override { calculator.divide(value); }
};

// Invoker Class
class CalculatorInvoker {
 private:
  std::vector<std::unique_ptr<Command>> history;

 public:
  void storeAndExecute(std::unique_ptr<Command> command) {
    command->execute();
    history.push_back(std::move(command));
  }
};

int main() {
  Calculator calculator;
  CalculatorInvoker invoker;

  // Add 10
  invoker.storeAndExecute(std::make_unique<AddCommand>(calculator, 10));

  // Subtract 5
  invoker.storeAndExecute(std::make_unique<SubtractCommand>(calculator, 5));

  // Multiply by 3
  invoker.storeAndExecute(std::make_unique<MultiplyCommand>(calculator, 3));

  // Divide by 2
  invoker.storeAndExecute(std::make_unique<DivideCommand>(calculator, 2));

  std::cout << "Final result: " << calculator.getResult() << std::endl;

  return 0;
}
