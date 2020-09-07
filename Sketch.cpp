#include "pch.hpp"
#include "Sketch.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {


	Sketch::Sketch() {
		
		featureCounter = 0;
		mCamDistance = nullptr;
	}

	Sketch::Sketch(int i) {
		
		setId(i);
		setType(cad_sketch);
		featureCounter = 0;

		mCamDistance = nullptr;
	}

	int Sketch::getFeatureCounter() {

		return featureCounter;
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


	Point* Sketch::addPoint(glm::vec3 point) {

		Point pointToAdd;

		int16_t pointSketchId = -1;

		// Already points in sketch
		if (Points.size() > 0) {

			if (mCamDistance == nullptr)
				throw std::runtime_error("Sketch.addPoint(): Sketch.mCamDistance is nullptr!");

			pointSketchId = sel::Selector::selectPoint(point, Points, *mCamDistance);

			// Point already exists
			if (pointSketchId >= 0) {

				if (&Points[pointSketchId] == nullptr)
					throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

				return &Points[pointSketchId];

			}
			// Point doesn't exist
			else {

				pointToAdd.pos = point;
				pointToAdd.setId(featureCounter);

				Points[featureCounter] = pointToAdd;

				if (&Points[pointSketchId] == nullptr)
					throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

				return &Points[featureCounter];

			}
		}
		// No points in sketch
		else {

			pointToAdd.pos = point;
			pointToAdd.setId(featureCounter);

			Points[featureCounter] = pointToAdd;

			if (&Points[pointSketchId] == nullptr)
				throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

			return &Points[featureCounter];

		}
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