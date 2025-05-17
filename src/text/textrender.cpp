#include "textrender.hpp"
#include "pch.hpp"

void coutVec3(glm::vec3 v) {
  std::cout << "x: " << v.x << " ";
  std::cout << "y: " << v.y << " ";
  std::cout << "z: " << v.z << " " << std::endl;
}

namespace CADERA_APP_NAMESPACE {
namespace txt {

TextRender::TextRender() : mFontSize{100.0f},
                           mAtlasSize{2864.0f},
                           mCursorPos{0.0f, 0.0f, 0.0f}, 
                           mCursorDirX{0.0f, 0.0f, 0.0f}, 
                           mCursorDirY{0.0f, 0.0f, 0.0f}, 
                           textCounter{0} {}

TextRender::~TextRender() {}

void TextRender::setFontSize(float s) { mFontSize = s; }

void TextRender::loadFont(std::string filePath) {

  Characters.clear();

  std::ifstream fontAtlasCsv;

  std::string row, cell, value;

  fontAtlasCsv.open(filePath);

  while (std::getline(fontAtlasCsv, row)) {

    std::stringstream ss(row);

    Character C;

    std::vector<float> values;

    while (ss.good()) {
      std::getline(ss, cell, ',');

      values.push_back(std::stof(cell));
    }

    C.xAdvance = values[1];
    C.x = values[2];
    C.y = values[5];
    C.width = (values[4] - values[2]);
    C.height = (values[5] - values[3]);
    C.textureCoords.x = values[6] / mAtlasSize;
    C.textureCoords.z = values[8] / mAtlasSize;
    C.textureCoords.y = 1.0f - values[7] / mAtlasSize;
    C.textureCoords.w = 1.0f - values[9] / mAtlasSize;

    Characters[(int)values[0]] = C;
  }
}

void TextRender::setCursorPos(glm::vec3 pos) { mCursorPos = pos; }

void TextRender::setPlane(glm::vec3 xDir, glm::vec3 yDir) {

  mCursorDirX = xDir;
  mCursorDirY = yDir;
}

void TextRender::addText(Text T) {

  mTexts[textCounter] = T;
  textCounter++;
}

void TextRender::addText(std::vector<Text> Ts) {

  for (const auto &T : Ts) {

    mTexts[textCounter] = T;
    textCounter++;
  }
}

void TextRender::clearTexts() { mTexts.clear(); }

std::vector<Vertex> TextRender::generateQuads(const glm::vec3& bgColor) {

  numChars = 0;

  std::vector<Vertex> vertices;

  for (const auto &T : mTexts) {

    setCursorPos(T.second.cursorPos);
    setPlane(glm::normalize(T.second.cursorDirX),
             glm::normalize(T.second.cursorDirY));

    for (const auto &c : T.second.text) {

      glm::vec3 p0, p1, p2, p3;

      // Character Atlas Values
      glm::vec3 width, height, xAdvance, xBear, yBear;

      glm::vec2 A, B, C, D;

      // Change to vectors in the plane
      xBear = T.second.textSize * Characters[c].x * mCursorDirX +
              T.second.offset.x * mCursorDirX;
      yBear = T.second.textSize * Characters[c].y * mCursorDirY +
              T.second.offset.y * mCursorDirY;
      width = T.second.textSize * Characters[c].width * mCursorDirX;
      height = T.second.textSize * Characters[c].height * mCursorDirY;
      xAdvance = T.second.textSize * Characters[c].xAdvance * mCursorDirX;

      // If Space, just advance the cursor and move on
      if (c == 32) {

        mCursorPos += xAdvance;
        continue;
      }

      // Texture Coordinates
      A = {Characters[c].textureCoords.x, Characters[c].textureCoords.w};
      B = {Characters[c].textureCoords.x, Characters[c].textureCoords.y};
      C = {Characters[c].textureCoords.z, Characters[c].textureCoords.y};
      D = {Characters[c].textureCoords.z, Characters[c].textureCoords.w};

      // Construct the 4 points of the Quad
      p0 = mCursorPos + xBear + yBear;
      p1 = p0 - height;
      p2 = p1 + width;
      p3 = p2 + height;

      // Generate Vertices
      Vertex V1 = {p0, T.second.textColor, bgColor, A};
      Vertex V2 = {p1, T.second.textColor, bgColor, B};
      Vertex V3 = {p2, T.second.textColor, bgColor, C};
      Vertex V4 = {p3, T.second.textColor, bgColor, D};

      vertices.push_back(V1);
      vertices.push_back(V2);
      vertices.push_back(V3);
      vertices.push_back(V4);

      // Advance the text cursor to the next position
      mCursorPos += xAdvance;

      numChars++;
    }
  }

  return vertices;
}

std::vector<uint32_t> TextRender::generateIndices() {

  std::vector<uint32_t> indices;

  for (int i = 0; i < numChars; i++) {
    indices.push_back(i * 4 + 0);
    indices.push_back(i * 4 + 1);
    indices.push_back(i * 4 + 2);
    indices.push_back(i * 4 + 2);
    indices.push_back(i * 4 + 3);
    indices.push_back(i * 4 + 0);
  }
  return indices;
}

} // namespace txt
} // namespace CADERA_APP_NAMESPACE
