#include "pch.hpp"
#include "Point.hpp"

namespace CADERA_APP_NAMESPACE {
	
	Point::Point() {
	
	}

	Point::Point(int id, glm::vec3 pos) {
	
		setId(id);
		this->pos = pos;

	
	}
}