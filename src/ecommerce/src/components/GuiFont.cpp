#include "GuiComponent.hpp"

GuiFont::GuiFont(const string &name, AssetType type)
    : fontName(name), type(type) {}

const string GuiFont::getFontStyle(const FontStyle &style) {
  static const string FONT_STYLES[] = {"Bold",         "BoldItalic",  "Italic",
                                       "Light",        "LightItalic", "Medium",
                                       "MediumItalic", "Regular"};
  return FONT_STYLES[int(style)];
}

string GuiFont::getFontPath(const GuiFont &font, const FontStyle &style) {
  return utils::getResourcePath(
      AssetFolder::FONTS,
      font.getFontName() + "/" + font.getFontName() + "-" + getFontStyle(style),
      font.getFontType());
  ;
}

vector<Font> &GuiFont::getFontVariants(const GuiFont &font) {
  static map<string, vector<Font>> fontVariantsMap;

  if (fontVariantsMap.find(font.fontName) == fontVariantsMap.end()) {
    fontVariantsMap[font.fontName] = vector<Font>(8);
  }

  return fontVariantsMap[font.fontName];
}

Font &GuiFont::getFont(const GuiFont &font, const int &baseSize,
                       FontStyle style) {
  vector<Font> fontVariants = getFontVariants(font);
  if (!IsFontReady(fontVariants[int(style)]))
    fontVariants[int(style)] =
        LoadFontEx(getFontPath(font, style).c_str(), baseSize, 0, 250);

  return fontVariants[int(style)];
}
