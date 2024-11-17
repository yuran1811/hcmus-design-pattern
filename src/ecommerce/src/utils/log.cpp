#include "index.hpp"

void utils::log::logging(bool newLine, const char *format, ...) {
  FILE *file = fopen(
      getResourcePath(AssetFolder::LOGS, "log", AssetType::TXT).c_str(), "a");
  if (!file) return;

  va_list args;
  va_start(args, format);
  vfprintf(file, format, args);
  va_end(args);

  if (newLine) fputc('\n', file);
  fclose(file);
}

void utils::log::CustomLog(int msgType, const char *text, va_list args) {
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
