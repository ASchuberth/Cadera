#include <gtest/gtest.h>
#include <render/Camera.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

// @brief Test Camera
TEST(Camera, SetZXView) {

  CADERA_APP_NAMESPACE::cam::Camera Cam;

  Cam.setZXView();

  EXPECT_EQ(Cam.pos.x, 0.0f);
  EXPECT_EQ(Cam.pos.y, 20.0f);
  EXPECT_EQ(Cam.pos.z, 0.0f);

  EXPECT_EQ(Cam.focus.x, 0.0f);
  EXPECT_EQ(Cam.focus.y, 0.0f);
  EXPECT_EQ(Cam.focus.z, 0.0f);

  EXPECT_EQ(Cam.cameraVec.x, 0.0f);
  EXPECT_EQ(Cam.cameraVec.y, -1.0f);
  EXPECT_EQ(Cam.cameraVec.z, 0.0f);

  EXPECT_EQ(Cam.up.x, 1.0f);
  EXPECT_EQ(Cam.up.y, 0.0f);
  EXPECT_EQ(Cam.up.z, 0.0f);
}