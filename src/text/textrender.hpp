#pragma once

namespace CADERA_APP_NAMESPACE {
namespace txt {

struct Text {
  std::string text;
  float textSize = 1.0f;
  glm::vec3 textColor;
  glm::vec3 cursorPos;
  glm::vec3 cursorDirX;
  glm::vec3 cursorDirY;
  glm::vec2 offset = {0.0f, 0.0f};
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 textColor;
  glm::vec3 backgroundColor;
  glm::vec2 texCoord;
};

struct Character {
  float width;
  float height;
  float x;
  float y;
  float xAdvance;
  glm::vec4 textureCoords;
};

class TextRender {

public:
  TextRender();
  ~TextRender();

  void setFontSize(float s);

  void loadFont(std::string filePath);

  void setCursorPos(glm::vec3 pos);
  void setPlane(glm::vec3 xDir, glm::vec3 yDir);

  void addText(Text T);
  void addText(std::vector<Text> Ts);
  void clearTexts();

  std::vector<Vertex> generateQuads(const glm::vec3& bgColor);
  std::vector<uint32_t> generateIndices();

private:
  std::map<unsigned char, Character> Characters;

  std::map<int, Text> mTexts;

  glm::vec3 mCursorPos;
  glm::vec3 mCursorDirX;
  glm::vec3 mCursorDirY;

  float mFontSize;
  float mAtlasSize;

  int numChars;
  int textCounter;
};
} // namespace txt
} // namespace CADERA_APP_NAMESPACE
