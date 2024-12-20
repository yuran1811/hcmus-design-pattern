#pragma once

#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <sstream>

#include "raylib.h"
#include "uuid_v4.hpp"
#include "qrcodegen.hpp"
#include "stb_image_write.hpp"

#include "../shared/index.hpp"

using std::ostringstream;

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

namespace utils {
string getResourcePath(AssetFolder, string, AssetType);

string uuid_v4();

string getDateTimeString();

float calcTextWrapHeight(const string &, const float &);

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
Color calcBreathColor(const Color &, const float &);
Color calcPulseColor(const Color &, const float &);
}  // namespace color

namespace ui {
bool mouseHoveredInBox(Rectangle);
bool mousePressedInBox(Rectangle, MouseButton);

bool mouseHoveredInCircle(Vector2, float);
bool mousePressedInCircle(Vector2, float, MouseButton);
}  // namespace ui

namespace qrcode {
string generateSVGQRCode(const string &);
pair<unsigned char *, int> generateQRCodeImage(const string &, const int &);
string saveQRCode(const string &, const string &, const int &);
}  // namespace qrcode
}  // namespace utils
