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
