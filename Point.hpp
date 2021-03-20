#pragma once
#include "Feature.hpp"
namespace CADERA_APP_NAMESPACE {

	class Point : public Feature {

	public:

		
		glm::vec3 pos;
		std::list<int16_t> relationIds;
		int noteId;

		Point();
		Point(int id, glm::vec3 pos);

	};
}