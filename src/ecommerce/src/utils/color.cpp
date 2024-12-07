#include "index.hpp"

Color utils::color::calcBreathColor(const Color &_, const float &time) {
  return {_.r, _.g, _.b, 255 * abs(sinf(time))};
}

Color utils::color::calcPulseColor(const Color &_, const float &time) {
  const float amplitude = 0.5f;
  const float frequency = 1.0f;
  const float fBrightness = sinf(frequency * time);
  float brightness = 0.5f + amplitude * abs(fBrightness);

  return {(unsigned char)(_.r * brightness), (unsigned char)(_.g * brightness),
          (unsigned char)(_.b * brightness), 255};
}
