#include <gtest/gtest.h>
#include <sketch/Point.hpp>

// @brief Test if id of Point without an input id is -1
TEST(Sketch, Point_Id_Basic) {

  CADERA_APP_NAMESPACE::Point P;

  EXPECT_EQ(P.getId(), -1);
}

// @brief Test if id of Point is set to the initialized value
TEST(Sketch, Point_Id_Set) {

  glm::vec3 v = {0.0f, 0.0f, 0.0f};

  CADERA_APP_NAMESPACE::Point P(3, v);

  EXPECT_EQ(P.getId(), 3);
}
