#pragma once

#include <chrono>
#include <ctime>
#include <cstdio>

#include "raylib.h"

#include "../shared/Common.hpp"

namespace utils {
namespace range {
bool is_out_range(float x, float l, float r);
bool is_in_range(float x, float l, float r);

float rand_in_range(int l, int r);
}  // namespace range

Font GetFont(const int &fontSize);

std::string getResourcePath(ASSET_FOLDER assetType, std::string resource,
                            ASSET_TYPE fileType);

void CustomLog(int msgType, const char *text, va_list args);

}  // namespace utils
