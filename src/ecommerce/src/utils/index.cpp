#include "index.hpp"

string utils::getResourcePath(AssetFolder assetType, string resource,
                              AssetType fileType = AssetType::NO_EXT) {
  const string assetFolder = ASSET_FOLDERS[static_cast<int>(assetType)];
  return ASSETS_PATH + assetFolder + resource +
         ASSET_FILE_TYPES[static_cast<int>(fileType)];
}
