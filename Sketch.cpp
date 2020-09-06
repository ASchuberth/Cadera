#include "pch.hpp"
#include "Sketch.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {


	Sketch::Sketch() {
		
		featureCounter = 0;

	}

	Sketch::Sketch(int i) {
		setId(i);
		setType(cad_sketch);
		featureCounter = 0;
	}

	void Sketch::setCameraDistance(float* camDistance) {
		mCamDistance = camDistance;
	}

	void Sketch::activatePointTool() {
		flags.set(skt_point_tool);
		flags.set(skt_tool_active);
	}

	void Sketch::deactivateTools() {
		flags.reset();
	}

	void Sketch::add(glm::vec3 point) {

		if (flags.test(skt_point_tool)) {
			addPoint(point);
		}

		featureCounter++;
	}

	void Sketch::addPoint(glm::vec3 point) {
		
		Points[featureCounter] = Point(featureCounter, point);


	}

	std::vector<glm::vec3> Sketch::getVertices() {

		std::vector<glm::vec3> vertices;

		for (const auto& p : Points) {
			vertices.push_back(p.second.pos);
		}

		return vertices;
	}

	





}
}