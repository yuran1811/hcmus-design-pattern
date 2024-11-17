#include "index.hpp"

string utils::qrcode::generateSVGQRCode(const string &data) {
  const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;
  const QrCode qr = QrCode::encodeText(data.c_str(), errCorLvl);

  const int border = 4;

  ostringstream sb;
  sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
        "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 "
        "0 ";
  sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2)
     << "\" stroke=\"none\">\n";
  sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
  sb << "\t<path d=\"";
  for (int y = 0; y < qr.getSize(); y++) {
    for (int x = 0; x < qr.getSize(); x++) {
      if (qr.getModule(x, y)) {
        if (x != 0 || y != 0) sb << " ";
        sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
      }
    }
  }
  sb << "\" fill=\"#000000\"/>\n";
  sb << "</svg>\n";
  return sb.str();
}

pair<unsigned char *, int> utils::qrcode::generateQRCodeImage(
    const string &data, const int &size) {
  const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;
  const QrCode qr = QrCode::encodeText(data.c_str(), errCorLvl);
  const int qrSize = qr.getSize();

  int scale = size / qrSize;
  scale += ((scale & 1) ^ (qrSize & 1)) ? 1 : 0;

  const int imgSize = qrSize * scale;
  const int bytesPerPixel = 3;

  unsigned char *imgData = new unsigned char[imgSize * imgSize * bytesPerPixel];

  for (int y = 0; y < qrSize; y++) {
    for (int x = 0; x < qrSize; x++) {
      bool isBlack = qr.getModule(x, y);
      unsigned char color = isBlack ? 0 : 255;

      for (int dy = 0; dy < scale; dy++) {
        for (int dx = 0; dx < scale; dx++) {
          int pixelIndex =
              ((y * scale + dy) * imgSize + (x * scale + dx)) * bytesPerPixel;
          imgData[pixelIndex + 0] = color;
          imgData[pixelIndex + 1] = color;
          imgData[pixelIndex + 2] = color;
        }
      }
    }
  }

  return {imgData, imgSize};
}

string utils::qrcode::saveQRCode(const string &filename, const string &data,
                                 const int &size) {
  const string path =
      getResourcePath(AssetFolder::QRCODE, filename, AssetType::PNG);

  const pair<unsigned char *, int> x = generateQRCodeImage(data, size);
  const unsigned char *png = x.first;
  const int imgSize = x.second;

  stbi_write_png(path.c_str(), imgSize, imgSize, 3, png, imgSize * 3);

  return path;
}
