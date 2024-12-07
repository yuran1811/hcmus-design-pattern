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

enum PaymentMethod { COD, CREDIT_CARD, PAYPAL, STRIPE };
static const string PAYMENT_METHODS[] = {"COD", "Credit Card", "PayPal",
                                         "Stripe"};
static const int PAYMENT_METHOD_COUNT = 4;

enum OrderStageState {
  SELECT_ITEM,
  ADDRESS_INPUT,
  PAYMENT_METHOD,
  PAYMENT,
  PACKAGING,
  SHIPPING,
  COMPLETED
};
static const string ORDER_STAGES[] = {
    "Select Item", "Address Input", "Payment Method", "Payment",
    "Packaging",   "Shipping",      "Completed"};

// Key: Item name, Value: Pair of Item and quantity
typedef unordered_map<string, pair<Item, int>> CartType;
