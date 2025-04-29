#include "Camera.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace cam {

Camera::Camera() {
  pos = {0.0f, 0.0f, 20.0f};
  focus = {0.0f, 0.0f, 0.0f};
  cameraVec = {0.0f, 0.0f, -1.0f};
  xpos = 0.0;
  ypos = 0.0;
  mouseRay = {0.0f, 0.0f, 0.0f};
  left = -10.0f;
  camDistance = 0.0f;

  up = {0.0f, 1.0f, 0.0f};

  cross = glm::cross(cameraVec, up);
}

void Camera::setXYView() {

  pos = {0.0f, 0.0f, 20.0f};
  focus = {0.0f, 0.0f, 0.0f};
  cameraVec = {0.0f, 0.0f, -1.0f};

  up = {0.0f, 1.0f, 0.0f};
  cross = glm::cross(cameraVec, up);
}

void Camera::setYZView() {

  pos = {20.0f, 0.0f, 0.0f};
  focus = {0.0f, 0.0f, 0.0f};
  cameraVec = {-1.0f, 0.0f, 0.0f};

  up = {0.0f, 0.0f, 1.0f};
  cross = glm::cross(cameraVec, up);
}

void Camera::setZXView() {

  pos = {0.0f, 20.0f, 0.0f};
  focus = {0.0f, 0.0f, 0.0f};
  cameraVec = {0.0f, -1.0f, 0.0f};

  up = {1.0f, 0.0f, 0.0f};
  cross = glm::cross(cameraVec, up);
}

void Camera::update() { camDistance = glm::length(pos - focus); }

void Camera::zoom(float yoffset) {

  if (flags.test(ortho)) {

    if (yoffset > 0 && left < -1.02) {
      left += 1.0f;
    } else if (yoffset < 0) {
      left -= 1.0f;
    }
  } else {

    glm::vec3 normalPos = {0.0f, pos.y, pos.z};

    if (yoffset > 0) {
      pos -= 1.0f * glm::normalize(cameraVec);
    } else if (yoffset < 0 && glm::length2(pos - focus) > 1.02f) {
      pos += 1.0f * glm::normalize(cameraVec);
    }
  }
}

void Camera::updateMouseRay(float x, float y, glm::mat4 modelMat,
                            glm::mat4 viewMat, glm::mat4 projMat,
                            uint32_t width, uint32_t height) {

  if (flags.test(ortho)) {
    mouseRay = glm::unProject(glm::vec3(x, y, 0.0f), modelMat, projMat,
                              glm::vec4(0.0f, 0.0f, width, height));
  } else {
    mouseRay = sel::calcCurrentRay(x, y, viewMat, projMat, width, height);
  }
}

/**
 * @brief
 *
 * @param origin Origin point of the plane.
 * This is used for calulating the point on the plane for projection view
 *
 * @param planeNormal The normal vector of the plane to pan on
 */
void Camera::pan(glm::vec3 origin, glm::vec3 planeNormal) {

  static glm::vec3 prevMouseRay = mouseRay;
  glm::vec3 p1, p2, diff;

  if (flags.test(mouseFirstPressed)) {

    prevMouseRay = mouseRay;
    flags.reset(mouseFirstPressed);
  }

  if (flags.test(ortho)) {

    diff = mouseRay - prevMouseRay;

    pos -= diff.y * up;
    pos -= diff.x * cross;
    focus -= diff.y * up;
    focus -= diff.x * cross;

  } else {

    p1 = sel::calcPOnPlane(prevMouseRay, origin, planeNormal, pos);
    p2 = sel::calcPOnPlane(mouseRay, origin, planeNormal, pos);

    diff = p2 - p1;

    pos -= diff;
    focus -= diff;
  }

  prevMouseRay = mouseRay;
}

} // namespace cam
} // namespace CADERA_APP_NAMESPACE
