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
