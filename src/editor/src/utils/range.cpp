#include "index.hpp"

bool utils::range::is_out_range(float x, float l, float r) {
  return l > x || x > r;
}

bool utils::range::is_in_range(float x, float l, float r) {
  return l <= x && x <= r;
}

float utils::range::rand_in_range(int l, int r) {
  return rand() % (r - l + 1) + l;
}