#include "Sketch.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {

Sketch::Sketch() {

  featureCounter = 1;
  addOrigin();

  mCamDistance = nullptr;
}

Sketch::Sketch(int i) {

  setId(i);
  setType(cad_sketch);
  featureCounter = 1;

  mCamDistance = nullptr;
}

int Sketch::getFeatureCounter() { return featureCounter; }

void Sketch::setCameraDistance(float *camDistance) {
  mCamDistance = camDistance;
}

void Sketch::activatePointTool() {
  deactivateTools();
  flags.set(skt_point_tool);
  flags.set(skt_tool_active);
}

void Sketch::activateNoteTool() {
  deactivateTools();
  flags.set(skt_note_tool);
  flags.set(skt_tool_active);
}

void Sketch::deactivateTools() {

  flags.reset(skt_point_tool);
  flags.reset(skt_note_tool);
  flags.reset(skt_tool_active);
  flags.set(skt_event_tool_deactivated);
}

void Sketch::add(glm::vec3 point) {

  if (flags.test(skt_point_tool)) {
    addPoint(point);
  }

  if (flags.test(skt_note_tool)) {
    addNotePoint(point);
  }
}

void Sketch::addOrigin() {

  Point Origin;

  Origin.pos = {0.0f, 0.0f, 0.0f};
  Origin.setId(0);
  Origin.Type = feat_origin;

  Points[0] = Origin;
}

void Sketch::addRelation(const std::vector<int> &ids, RelationType Type) {

  Relation newRelation;

  newRelation.mId = featureCounter;

  for (const auto &id : ids) {
    newRelation.mFeatureIds.push_back(id);

    Points[id].relationIds.push_back(newRelation.mId);
  }

  newRelation.mType = Type;

  mRelations[featureCounter] = newRelation;

  featureCounter++;
}

void Sketch::deleteRelation(int id) { mRelations.erase(id); }

void Sketch::clearRelations() { mRelations.clear(); }

size_t Sketch::numRelations() { return mRelations.size(); }

Point *Sketch::addPoint(glm::vec3 point) {

  Point pointToAdd;

  pointToAdd.pos = point;
  pointToAdd.setId(featureCounter);
  pointToAdd.Type = feat_point;

  Points[featureCounter] = pointToAdd;

  if (&Points[featureCounter] == nullptr)
    throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

  featureCounter++;

  return &Points[featureCounter - 1];
}

Point *Sketch::addConstructionPoint(glm::vec3 point) {

  Point pointToAdd;

  pointToAdd.pos = point;
  pointToAdd.setId(featureCounter);
  pointToAdd.Type = feat_construction;

  Points[featureCounter] = pointToAdd;

  if (&Points[featureCounter] == nullptr)
    throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

  featureCounter++;

  return &Points[featureCounter - 1];
}

Point *Sketch::addNotePoint(glm::vec3 point) {

  Point pointToAdd;

  pointToAdd.pos = point;
  pointToAdd.setId(featureCounter);
  pointToAdd.Type = feat_note;
  pointToAdd.noteId = featureCounter + 1;

  Points[featureCounter] = pointToAdd;

  if (&Points[featureCounter] == nullptr)
    throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

  featureCounter++;

  txt::Text T;

  T.textSize = 1.0f;
  T.cursorPos = point;
  T.cursorDirX = {1.0f, 0.0f, 0.0f};
  T.cursorDirY = {0.0f, 1.0f, 0.0f};
  T.backgroundColor = {0.9f, 0.9f, 0.9f};
  T.textColor = {1.0f, 0.0f, 0.0f};
  T.text = noteText;

  Notes[featureCounter] = T;

  featureCounter++;

  return &Points[featureCounter - 2];
}

void Sketch::movePoints(std::map<int, Point> ids, const glm::vec3 &POnPlane,
                        const bool &isFirstClick) {

  static glm::vec3 initPoint = POnPlane;

  if (isFirstClick)
    initPoint = POnPlane;

  glm::vec3 difference = POnPlane - initPoint;

  if (glm::length2(difference) < .001)
    return;

  for (const auto &id : ids) {

    if (id.first == 0)
      continue;

    Points[id.first].pos += difference;

    if (Points[id.first].Type == feat_note) {

      Notes[Points[id.first].noteId].cursorPos = Points[id.first].pos;
    }
  }

  flags.set(skt_points_have_moved);

  initPoint = POnPlane;
}

void Sketch::deletion(std::vector<int> ids) {

  for (const auto &id : ids) {

    if (id == 0)
      continue;

    if (Points[id].Type == feat_note && Points[id].noteId != -1) {
      Notes.erase(Points[id].noteId);
    }

    for (const auto &rId : Points[id].relationIds) {
      mRelations[rId].removeId(id);

      if (mRelations[rId].mFeatureIds.size() == 1) {

        Points[*mRelations[rId].mFeatureIds.begin()].removeRelationId(rId);
        mRelations.erase(rId);
      }
    }

    Points.erase(id);
  }
}

std::vector<glm::vec3> Sketch::getGridLine() { return mGrid.line; }

std::vector<GridRotationAxis> Sketch::getGridAxii() {
  return mGrid.createGridInstanceAxii();
}

std::vector<glm::vec3> Sketch::getVertices(std::vector<glm::vec3> &colors) {

  std::vector<glm::vec3> vertices;
  colors.clear();

  for (const auto &p : Points) {
    vertices.push_back(p.second.pos);

    if (p.second.Type == feat_point) {
      colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    } else if (p.second.Type == feat_construction) {
      colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    } else if (p.second.Type == feat_note) {
      colors.push_back(glm::vec3(1.0f, 0.8f, 0.0f));
    } else if (p.second.Type == feat_origin) {
      colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    }
  }

  return vertices;
}

std::vector<txt::Text> Sketch::getRelationTexts() {

  std::vector<txt::Text> Texts;
  std::map<int, int> pointRelCounter;

  for (const auto &R : mRelations) {

    for (const auto &id : R.second.mFeatureIds) {
      txt::Text T;

      T.textSize = 0.3f;
      T.backgroundColor = {0.0f, 0.0f, 1.0f};
      T.textColor = {1.0f, 1.0f, 1.0f};
      T.cursorDirX = {0.0f, 0.0f, 1.0f};
      T.cursorDirY = {0.0f, 1.0f, 0.0f};
      T.cursorPos = Points[id].pos;
      T.offset = {0.1f, 0.1f};
      T.offset.x += 0.3f * (float)pointRelCounter[id];

      if (R.second.mType == rel_vertical) {
        T.text = "|";
      } else if (R.second.mType == rel_horizontal) {
        T.text = "_";
      } else if (R.second.mType == rel_coincident) {
        T.text = "C";
      }

      Texts.push_back(T);

      pointRelCounter[id]++;
    }
  }

  return Texts;
}

} // namespace sketch
} // namespace CADERA_APP_NAMESPACE