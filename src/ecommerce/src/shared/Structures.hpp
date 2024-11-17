#pragma once

#include <iostream>

using std::max;
using std::ostream;
using std::string;
using std::to_string;

struct Size {
  float width;
  float height;

  Size() : width(0), height(0){};
  Size(float width, float height) : width(width), height(height){};
  Size(float width) : Size(width, width){};
  Size(const Size& other) : Size(other.width, other.height){};
  ~Size() = default;

  Size& operator=(const Size& other) {
    width = other.width;
    height = other.height;
    return *this;
  }
};

struct ScreenSize : public Size {
  ScreenSize() : Size(){};
  ScreenSize(float width) : Size(width){};
  ScreenSize(float width, float height) : Size(width, height){};
  ScreenSize(const ScreenSize& other) : Size(other.width, other.height){};
  ~ScreenSize() = default;

  ScreenSize& operator=(const ScreenSize& other) {
    width = other.width;
    height = other.height;
    return *this;
  }
};

struct Coord {
  int x, y;

  Coord() : x(0), y(0) {}
  Coord(int x, int y) : x(x), y(y) {}
  Coord(int x) : Coord(x, x) {}
  Coord(const Coord& other) : x(other.x), y(other.y) {}

  bool operator==(const Coord& other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Coord& other) const { return !(*this == other); }
  bool operator<(const Coord& other) const {
    return y < other.y || (y == other.y && x < other.x);
  }

  Coord operator-() const { return Coord(-x, -y); }

  Coord operator+(const Coord& other) const {
    return Coord(x + other.x, y + other.y);
  }
  Coord operator-(const Coord& other) const {
    return Coord(x - other.x, y - other.y);
  }
  Coord operator*(int scalar) const { return Coord(x * scalar, y * scalar); }
  Coord operator/(int scalar) const { return Coord(x / scalar, y / scalar); }

  Coord& operator+=(const Coord& other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Coord& operator-=(const Coord& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Coord& operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Coord& operator/=(int scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  friend ostream& operator<<(std::ostream& os, const Coord& coord) {
    os << "(" << coord.x << ", " << coord.y << ")";
    return os;
  }
};

struct Price {
  long long value;
  int floatPart;

  Price() : value(0), floatPart(1) {}
  Price(long long value, int floatPart) : value(value), floatPart(floatPart) {}
  Price(long long value) : Price(value, 1) {}
  Price(const Price& other) : Price(other.value, other.floatPart) {}

  operator float() const { return 1.f * value / floatPart; }

  string format() const {
    return to_string(value / floatPart) + "." + to_string(value % floatPart);
  }

  Price operator*(int scalar) const { return Price(value * scalar, floatPart); }

  Price& operator=(const Price& other) {
    value = other.value;
    floatPart = other.floatPart;
    return *this;
  }

  Price operator+(const Price& other) const {
    int maxFloatPart = max(floatPart, other.floatPart);
    long long newValue = value * maxFloatPart / floatPart +
                         other.value * maxFloatPart / other.floatPart;
    while (newValue % 10 == 0 && maxFloatPart > 10) {
      newValue /= 10;
      maxFloatPart /= 10;
    }
    return Price(newValue, maxFloatPart);
  }

  Price& operator+=(const Price& other) {
    *this = *this + other;
    return *this;
  }

  Price operator-(const Price& other) const {
    return *this + Price(-other.value, other.floatPart);
  }

  Price& operator-=(const Price& other) {
    *this = *this - other;
    return *this;
  }

  friend ostream& operator<<(ostream& os, const Price& price) {
    os << price.format();
    return os;
  }
};

struct Item {
  string name;
  Price price;
};

struct BitState {
  unsigned long long value;

  BitState() : value(0) {}
  BitState(unsigned long long value) : value(value) {}
  BitState(const BitState& other) : value(other.value) {}

  bool isSet(unsigned int bit) const { return value & (1 << bit); }

  void reset() { value = 0; }
  void set(unsigned int bit) { value |= (1 << bit); }
  void unset(unsigned int bit) { value &= ~(1 << bit); }
  void toggle(unsigned int bit) { value ^= (1 << bit); }
};

struct Voucher {
  string name;
  int discount;
};

template <class T>
struct Range {
  T start;
  T end;

  Range() : start(0), end(0) {}
  Range(T start, T end) : start(start), end(end) {}
  Range(T start) : Range(start, start) {}
  Range(const Range& other) : Range(other.start, other.end) {}

  bool isInRange(T value) const { return start <= value && value <= end; }
};
