#include "model.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

Model::Model() : mId{0} {}

Model::Model(int id) { mId = id; }

void Model::setId(int id) { mId = id; }

int Model::getId() { return mId; }

void Model::setType(ModelType type) { mType = type; }

ModelType Model::getType() { return mType; }

void Model::notify() {

  for (const auto& observer : observers) {
    std::cout << "Model notifies." << std::endl;
    observer->onNotify(mId, mRenderItems);
  }
}

std::vector<glm::vec3> Model::getGridLine() { return std::vector<glm::vec3>(); }

std::vector<GridRotationAxis> Model::getGridAxii() {
  return std::vector<GridRotationAxis>();
}

std::vector<txt::Text> Model::getRelationTexts() {
  return std::vector<txt::Text>();
}

std::vector<glm::vec3> Model::getVertices() { return std::vector<glm::vec3>(); }

std::vector<glm::vec3> Model::getVertices(std::vector<glm::vec3> &colors) {
  return std::vector<glm::vec3>();
}

} // namespace CADERA_APP_NAMESPACE
