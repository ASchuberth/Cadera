#include "Selection.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sel {

float calcRatioToPlane(glm::vec3 currentRay, glm::vec3 cameraToPlaneDist,
                       glm::vec3 planeOrigin, glm::vec3 planeNormal) {

  float ratio;

  glm::vec3 rayProjToPlaneNormalDist;
  glm::vec3 cameraToNormalPlaneDist;

  cameraToNormalPlaneDist =
      (glm::dot(cameraToPlaneDist, planeNormal) * planeNormal);
  rayProjToPlaneNormalDist = (glm::dot(currentRay, planeNormal) * planeNormal);

  ratio = std::fabs(glm::length(cameraToNormalPlaneDist) /
                    glm::length(rayProjToPlaneNormalDist));

  return ratio;
}

glm::vec3 calcPOnPlane(glm::vec3 currentRay, glm::vec3 origin, glm::vec3 normal,
                       glm::vec3 pos) {

  glm::vec3 cameraToPlaneDist;
  float ratio;
  glm::vec3 vecCamToPOnPlane;
  glm::vec3 POnPlaneCoord;

  cameraToPlaneDist = {pos.x - origin.x, pos.y - origin.y, pos.z - origin.z};

  ratio = calcRatioToPlane(currentRay, cameraToPlaneDist, origin, normal);

  vecCamToPOnPlane = ratio * currentRay;
  POnPlaneCoord = cameraToPlaneDist + vecCamToPOnPlane + origin;

  return POnPlaneCoord;
}

glm::vec2 normalizeToVulkanCoords(glm::vec2 screenCoords, uint32_t surfaceWidth,
                                  uint32_t surfaceHeight) {
  glm::vec2 normCoords;

  normCoords.x = 2.0f * screenCoords.x / static_cast<float>(surfaceWidth) - 1.0;
  normCoords.y =
      2.0f * screenCoords.y / static_cast<float>(surfaceHeight) - 1.0;

  return normCoords;
}

glm::vec4 clipToEyeSpace(glm::vec4 clipCoords, glm::mat4 projMat) {
  glm::vec4 eyeCoords;
  glm::mat4 inverseProjection;

  inverseProjection = glm::inverse(projMat);
  eyeCoords = inverseProjection * clipCoords;
  eyeCoords.z = -1.0f;
  eyeCoords.w = 0.0f;

  return eyeCoords;
}

glm::vec3 toWorldCoord(glm::vec4 eyeCoords, glm::mat4 viewMat) {
  glm::vec3 mouseRay;
  glm::vec4 rayWorld;
  glm::mat4 inverseView;

  inverseView = glm::inverse(viewMat);
  rayWorld = inverseView * eyeCoords;
  mouseRay = {rayWorld.x, rayWorld.y, rayWorld.z};
  mouseRay = glm::normalize(mouseRay);

  return mouseRay;
}

glm::vec3 calcCurrentRay(float x, float y, glm::mat4 viewMat, glm::mat4 projMat,
                         uint32_t width, uint32_t height) {
  glm::vec2 screenCoords;
  glm::vec2 normCoords;
  glm::vec4 clipCoords;
  glm::vec4 eyeCoords;

  glm::vec3 currentRay;

  screenCoords = {x, y};

  normCoords = normalizeToVulkanCoords(screenCoords, width, height);
  clipCoords = {normCoords.x, normCoords.y, -1.0f, 1.0f};
  eyeCoords = clipToEyeSpace(clipCoords, projMat);

  currentRay = toWorldCoord(eyeCoords, viewMat);

  return currentRay;
}

Selector::Selector() {

  point = {0.0f, 0.0f, 0.0f};

  flags.set(select_first_click);
}

void Selector::setActiveSketch(sketch::Sketch *pSketch) {

  pActiveSketch = pSketch;
}

void Selector::select(glm::vec3 mouseRay, glm::vec3 origin, glm::vec3 normal,
                      glm::vec3 pos, glm::vec3 cross, bool isOrtho) {

  if (isOrtho) {
    glm::vec3 up = glm::cross(cross, normal);

    glm::vec3 x = mouseRay.x * cross;
    glm::vec3 y = mouseRay.y * up;

    glm::vec3 posOnPlane = origin + (up + cross) * pos;
    point = posOnPlane + x + y;
  } else {
    point = calcPOnPlane(mouseRay, origin, normal, pos);
  }
}

int Selector::selectPoint(glm::vec3 pointToAdd, std::map<int, Point> &points,
                          float skScale) {

  std::vector<int> sortedIndices;
  double sensitivity = skScale / 800.0f;

  for (const auto &point : points) {
    glm::vec3 pointVec;
    double length;

    pointVec = pointToAdd - point.second.pos;

    length = glm::length2(pointVec);

    if (length < sensitivity) {
      return point.first;
    }
  }

  return -1;
}

void Selector::update(std::map<int, Point> points) {

  selectedPoints.clear();

  for (const auto &point : points) {

    selectedPoints[point.first] = point.second;
  }
}

int Selector::add(glm::vec3 pointToAdd, std::map<int, Point> &points,
                  float skScale) {

  // Clear selected points if CTRL isn't pressed
  if (!flags.test(select_isCTRL)) {
    selectedPoints.clear();
    setFlags();
  }

  pointJustAdded = false;

  int selectedPointId = selectPoint(pointToAdd, points, skScale);

  // If a point is selected
  if (selectedPointId >= 0) {

    // If not in selectedPoints, add it, else remove it
    if (selectedPoints.find(selectedPointId) == selectedPoints.end()) {

      selectedPoints[selectedPointId] = points[selectedPointId];
      setFlags();

      pointJustAdded = true;

      return selectedPointId;
    }
  }

  // If not CTRL and selectedPointId < 0
  if (!flags.test(select_isCTRL)) {

    selectedPoints.clear();
    setFlags();
  }

  return selectedPointId;
}

int Selector::remove(glm::vec3 pointToRemove, std::map<int, Point> &points,
                     float skScale) {
  if (!flags.test(select_isCTRL))
    return -1;

  if (pointJustAdded)
    return -1;

  int selectedPointId = selectPoint(pointToRemove, points, skScale);

  if (selectedPointId >= 0) {

    selectedPoints.erase(selectedPointId);
    setFlags();

    return selectedPointId;
  }

  return -1;
}

bool Selector::existingPoint(glm::vec3 point) {

  if (pActiveSketch == nullptr)
    throw std::runtime_error(
        "Selection.existingPoint(): pActiveSketch is not set!");

  int16_t pointSketchId = -1;

  // If there are points in sketch
  if (pActiveSketch->Points.size() > 0) {

    if (pActiveSketch->mCamDistance == nullptr)
      throw std::runtime_error(
          "Selection.existingPoint(): pActiveSketch->mCamDistance is nullptr!");

    pointSketchId = sel::Selector::selectPoint(point, pActiveSketch->Points,
                                               *pActiveSketch->mCamDistance);

    // Point already exists
    if (pointSketchId >= 0) {

      if (&pActiveSketch->Points[pointSketchId] == nullptr)
        throw std::runtime_error(
            "Sketch.addPoint(): Funtion returned a nullptr!");

      return true;

    } else {
      return false;
    }
  }

  // No points in sketch
  return false;
}

void Selector::setFlags() {

  if (selectedPoints.size() == 1) {

    flags.set(select_single_point);

    flags.reset(select_double_point);
    flags.reset(select_multi_point);

  } else if (selectedPoints.size() == 2) {

    flags.set(select_double_point);

    flags.reset(select_single_point);
    flags.reset(select_multi_point);

  } else if (selectedPoints.size() >= 3) {

    flags.set(select_multi_point);

    flags.reset(select_single_point);
    flags.reset(select_double_point);

  } else {

    flags.reset(select_single_point);
    flags.reset(select_double_point);
    flags.reset(select_multi_point);
  }
}

void Selector::clear() { selectedPoints.clear(); }

std::vector<glm::vec3> Selector::getVertices() {

  std::vector<glm::vec3> vertices;

  for (const auto &p : selectedPoints) {
    vertices.push_back(p.second.pos);
  }

  return vertices;
}

std::vector<int> Selector::getSelectedPointIds() {

  std::vector<int> ids;

  for (auto &P : selectedPoints) {
    ids.push_back(P.second.getId());
  }

  return ids;
}

} // namespace sel
} // namespace CADERA_APP_NAMESPACE