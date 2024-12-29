#pragma once

#include <cmath>
#include <iostream>

using std::max;
using std::ostream;
using std::string;
using std::to_string;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;

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

  friend ostream& operator<<(std::ostream& os, const Size& size) {
    os << "(" << size.width << ", " << size.height << ")";
    return os;
  }
};

struct ScreenSize : public Size {
  ScreenSize() : Size(){};
  ScreenSize(float width) : Size(width){};
  ScreenSize(float width, float height) : Size(width, height){};
  ScreenSize(const ScreenSize& other) : Size(other.width, other.height){};
  ~ScreenSize() = default;

  ScreenSize& operator=(const ScreenSize& other) {
    Size::operator=(other);
    return *this;
  }

  friend ostream& operator<<(std::ostream& os, const ScreenSize& size) {
    os << (Size)size;
    return os;
  }
};

struct Coord {
  int x, y;

  Coord() : x(0), y(0) {}
  Coord(int x, int y) : x(x), y(y) {}
  Coord(int x) : Coord(x, x) {}
  Coord(const Coord& other) : x(other.x), y(other.y) {}
  ~Coord() = default;

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
  ll value;
  uint decimal;

  Price() : value(0), decimal(0) {}
  Price(ll value, int decimal) : value(value), decimal(abs(decimal)) {}
  Price(ll value) : Price(value, 0) {}
  Price(const Price& other) : Price(other.value, other.decimal) {}
  ~Price() = default;

  operator double() const { return 1.0 * value / pow(10, decimal); }

  string format() const {
    const ll exp = pow(10, decimal);
    return to_string(value / exp) + "." + to_string(value % exp);
  }

  bool operator==(const Price& other) const {
    return value == other.value && decimal == other.decimal;
  }

  bool operator!=(const Price& other) const { return !(*this == other); }

  bool operator<(const Price& other) const {
    return value * pow(10, -decimal + other.decimal) < other.value;
  }
  friend bool operator<(const Price& price, int value) {
    return price < Price(value);
  }

  bool operator>(const Price& other) const { return other < *this; }
  friend bool operator>(const Price& price, int value) {
    return price > Price(value);
  }

  Price operator*(int scalar) const { return Price(value * scalar, decimal); }

  Price operator*(float scalar) const {
    // Scale the value based on scalar
    ll newValue = static_cast<ll>(round(value * scalar));
    // Adjust the decimal for potential fractional results
    int newDecimal = decimal;

    // If scalar introduces decimal precision, increase `decimal`
    string scalarStr = to_string(scalar);
    size_t decimalPos = scalarStr.find('.');
    if (decimalPos != string::npos) {
      int scalarDecimal = scalarStr.size() - decimalPos - 1;
      newDecimal += scalarDecimal;
    }

    // Normalize to remove trailing zeroes
    while (newValue % 10 == 0 && newDecimal > 0) {
      newValue /= 10;
      newDecimal--;
    }

    return Price(newValue, newDecimal);
  }

  Price& operator=(const Price& other) {
    value = other.value;
    decimal = other.decimal;
    return *this;
  }

  Price operator+(const Price& other) const {
    int maxDecimal = max(decimal, other.decimal);
    const ll rawValue = value * pow(10, maxDecimal - decimal) +
                        other.value * pow(10, maxDecimal - other.decimal);
    ll newValue = abs(rawValue);
    while (newValue % 10 == 0 && maxDecimal > 1) {
      newValue /= 10;
      maxDecimal--;
    }
    return Price(newValue * (rawValue > 0 ? 1 : -1), maxDecimal);
  }

  Price& operator+=(const Price& other) {
    *this = *this + other;
    return *this;
  }

  Price operator-(const Price& other) const {
    return *this + Price(-other.value, other.decimal);
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
  ull value;

  BitState() : value(0) {}
  BitState(ll value) : value(abs(value)) {}
  BitState(const BitState& other) : value(other.value) {}
  ~BitState() = default;

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

struct Coupon {
  string code;

  Price discount;     // Discount percentage or flat amount
  bool isPercentage;  // true = percentage, false = flat amount

  time_t expiryDate;
  int usageLimit;
};

template <class T>
struct Range {
  T start;
  T end;

  Range() : start(0), end(0) {}
  Range(T start, T end) : start(start), end(end) {}
  Range(T start) : Range(start, start) {}
  Range(const Range& other) : Range(other.start, other.end) {}
  ~Range() = default;

  bool isInRange(T value) const { return start <= value && value <= end; }
};
