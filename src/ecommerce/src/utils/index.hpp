#pragma once

#include <ctime>
#include <cmath>
#include <cstdio>

#include <sstream>
#include <chrono>

#include "raylib.h"
#include "qrcodegen.hpp"
#include "stb_image_write.hpp"

#include "../shared/index.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

using std::ostringstream;

namespace utils {
string getResourcePath(AssetFolder, string, AssetType);

namespace range {
template <class T>
bool is_out_range(T x, T l, T r) {
  return l > x || x > r;
}

template <class T>
bool is_in_range(T x, T l, T r) {
  return l <= x && x <= r;
}

template <class T>
bool is_in_range(Range<T> range, T x) {
  return range.isInRange(x);
}

template <class T>
T rand_in_range(T l, T r) {
  return rand() % int((r - l + 1) + l);
}
}  // namespace range

namespace log {
void logging(bool, const char *, ...);
void CustomLog(int, const char *, va_list);
}  // namespace log

namespace color {
Color calcBreathColor(const Color &_, const float &time);
Color calcPulseColor(const Color &_, const float &time);
}  // namespace color

namespace qrcode {
string generateSVGQRCode(const string &);
pair<unsigned char *, int> generateQRCodeImage(const string &, const int &);
string saveQRCode(const string &, const string &, const int &);
}  // namespace qrcode

namespace ui {
bool mousePressedInBox(Rectangle, MouseButton);
}

}  // namespace utils
