#pragma once
#include "Feature.hpp"
namespace CADERA_APP_NAMESPACE {

	class Point : public Feature {

	public:

		
		glm::vec3 pos;

		Point();
		Point(int id, glm::vec3 pos);

	};
}