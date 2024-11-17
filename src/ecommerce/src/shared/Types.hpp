#pragma once

#include <string>
#include <unordered_map>

#include "Structures.hpp"

using std::pair;
using std::string;
using std::unordered_map;

enum class AssetFolder {
  FONTS,
  ICONS,
  IMAGES,
  QRCODE,
  SOUNDS,
  TEXTURES,
  LOGS,
  DATA
};
static const string ASSET_FOLDERS[] = {"/fonts/",  "/icons/",  "/images/",
                                       "/qrcode/", "/sounds/", "/textures/",
                                       "/logs/",   "/data/"};

enum class AssetType { NO_EXT, TTF, PNG, JPG, SVG, WAV, MP3, TXT };
static const string ASSET_FILE_TYPES[] = {
    "", ".ttf", ".png", ".jpg", ".svg", ".wav", ".mp3", ".txt",
};

enum PaymentMethod { CREDIT_CARD, PAYPAL, COD };
static const string PAYMENT_METHODS[] = {"Credit Card", "PayPal", "COD"};

enum OrderStageState {
  SELECT_ITEM,
  ADDRESS_INPUT,
  PAYMENT_METHOD,
  PAYMENT,
  PACKAGING,
  SHIPPING,
  COMPLETED
};

// Key: Item name, Value: Pair of Item and quantity
typedef unordered_map<string, pair<Item, int>> CartType;
