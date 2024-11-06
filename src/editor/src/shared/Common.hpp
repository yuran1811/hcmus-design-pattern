#pragma once

#include <iostream>

#include "Size.hpp"

static ScreenSize SCREEN_SIZE(756, 480);

const std::string APP_TITLE(
    "Text Editor using Raylib and Command Design Pattern");

enum class ASSET_FOLDER { FONTS, ICONS, IMAGES, SOUNDS, TEXTURES, LOGS, DATA };
enum class ASSET_TYPE { NO_EXT, TTF, PNG, JPG, WAV, MP3, TXT };

const std::string ASSETS_PATH("./public");
const std::string ASSET_FOLDERS[] = {"/fonts/",  "/icons/",    "/images/",
                                     "/sounds/", "/textures/", "/logs/",
                                     "/data/"};
const std::string ASSET_FILE_TYPES[] = {
    "", ".ttf", ".png", ".jpg", ".wav", ".mp3", ".txt",
};

const int TARGET_FPS = 60;

const int NUM_COLOR_LEVELS = 5;
const int NUM_COLORS = NUM_COLOR_LEVELS * 11;
