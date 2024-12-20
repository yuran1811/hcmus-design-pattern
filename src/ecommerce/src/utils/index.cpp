#include "index.hpp"

string utils::getResourcePath(AssetFolder assetType, string resource,
                              AssetType fileType = AssetType::NO_EXT) {
  const string assetFolder = ASSET_FOLDERS[static_cast<int>(assetType)];
  return ASSETS_PATH + assetFolder + resource +
         ASSET_FILE_TYPES[static_cast<int>(fileType)];
}

string utils::uuid_v4() {
  static UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
  static UUIDv4::UUID test;
  static char uuid[37];

  test = uuidGenerator.getUUID();
  test.str(uuid);
  uuid[strlen(uuid) - 1] = '\0';

  return string(uuid);
}

string utils::getDateTimeString() {
  const time_t now = time(0);
  const tm *ltm = localtime(&now);

  ostringstream oss;
  oss << ltm->tm_mday << "-" << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year;
  oss << ", ";
  oss << ltm->tm_hour << ":" << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min
      << ":" << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec;

  return oss.str();
}

float utils::calcTextWrapHeight(const string &text, const float &boundsWidth) {
  const Font &font = GetFontDefault();
  const float spacing = 2.f;
  const float scaleFactor = 20.f / 10.f;
  const int length = text.size();

  float textOffsetY = 0;
  float textOffsetX = 0.0f;

  enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
  bool wordWrap = true;
  int state = MEASURE_STATE;

  int startLine = -1;  // Index where to begin drawing (where a line begins)
  int endLine = -1;    // Index where to stop drawing (where a line ends)
  int selectStart = 0;
  int selectLength = 0;
  int lastk = -1;  // Holds last value of the character position

  for (int i = 0, k = 0; i < length; i++, k++) {
    // Get next codepoint from byte string and glyph index in font
    int codepointByteCount = 0;
    int codepoint = GetCodepoint(&text[i], &codepointByteCount);
    int index = GetGlyphIndex(font, codepoint);

    // NOTE: Normally we exit the decoding sequence as soon as a bad byte is
    // found (and return 0x3f) but we need to draw all of the bad bytes using
    // the '?' symbol moving one byte
    if (codepoint == 0x3f) codepointByteCount = 1;
    i += (codepointByteCount - 1);

    float glyphWidth = 0;
    if (codepoint != '\n') {
      glyphWidth = (font.glyphs[index].advanceX == 0)
                       ? font.recs[index].width * scaleFactor
                       : font.glyphs[index].advanceX * scaleFactor;

      if (i + 1 < length) glyphWidth = glyphWidth + spacing;
    }

    if (state == MEASURE_STATE) {
      if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
        endLine = i;

      if ((textOffsetX + glyphWidth) > boundsWidth) {
        endLine = (endLine < 1) ? i : endLine;
        if (i == endLine) endLine -= codepointByteCount;
        if ((startLine + codepointByteCount) == endLine)
          endLine = (i - codepointByteCount);

        state = !state;
      } else if ((i + 1) == length) {
        endLine = i;
        state = !state;
      } else if (codepoint == '\n')
        state = !state;

      if (state == DRAW_STATE) {
        textOffsetX = 0;
        i = startLine;
        glyphWidth = 0;

        // Save character position when we switch states
        int tmp = lastk;
        lastk = k - 1;
        k = tmp;
      }
    } else {
      if (codepoint == '\n') {
        if (!wordWrap) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }
      } else {
        if (!wordWrap && ((textOffsetX + glyphWidth) > boundsWidth)) {
          textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
          textOffsetX = 0;
        }

        if ((selectStart >= 0) && (k >= selectStart) &&
            (k < (selectStart + selectLength))) {
        }
      }

      if (wordWrap && (i == endLine)) {
        textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
        textOffsetX = 0;
        startLine = endLine;
        endLine = -1;
        glyphWidth = 0;
        selectStart += lastk - k;
        k = lastk;

        state = !state;
      }
    }

    if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;
  }

  return textOffsetY;
}
