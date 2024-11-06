#include "index.hpp"

Button::Button() : x(0), y(0), width(0), height(0), text("") {}

Button::Button(int x, int y, int width, int height, const std::string& text)
    : x(x), y(y), width(width), height(height), text(text) {}

Button::Button(const Button& b) : Button(b.x, b.y, b.width, b.height, b.text) {
  command = b.command;
}

Button::~Button() { delete command; }

void Button::render() {
  GuiButton((Rectangle){x, y, width, height}, text.c_str());
}