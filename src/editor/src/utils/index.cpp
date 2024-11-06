#include "index.hpp"

Font utils::GetFont(const int &fontSize) {
  return LoadFontEx(
      utils::getResourcePath(ASSET_FOLDER::FONTS, "JetBrainsMonoNerd-Regular",
                             ASSET_TYPE::TTF)
          .c_str(),
      fontSize, nullptr, 0);
}

std::string utils::getResourcePath(ASSET_FOLDER assetType, std::string resource,
                                   ASSET_TYPE fileType = ASSET_TYPE::NO_EXT) {
  const std::string assetFolder = ASSET_FOLDERS[static_cast<int>(assetType)];
  return ASSETS_PATH + assetFolder + resource +
         ASSET_FILE_TYPES[static_cast<int>(fileType)];
}

void utils::CustomLog(int msgType, const char *text, va_list args) {
  char timeStr[64] = {0};
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);

  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
  printf("[%s] ", timeStr);

  switch (msgType) {
    case LOG_INFO:
      printf("[INFO] : ");
      break;
    case LOG_ERROR:
      printf("[ERROR]: ");
      break;
    case LOG_WARNING:
      printf("[WARN] : ");
      break;
    case LOG_DEBUG:
      printf("[DEBUG]: ");
      break;
    default:
      break;
  }

  vprintf(text, args);
  printf("\n");
}
