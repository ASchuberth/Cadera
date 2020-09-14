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

	}

	void Sketch::addRelation(const std::vector<int>& ids, RelationType Type) {

		Relation newRelation;

		newRelation.mId = featureCounter;

		for (const auto& id : ids) {
			newRelation.mFeatureIds.push_back(id);
		}

		newRelation.mType = Type;

		mRelations[featureCounter] = newRelation;

		featureCounter++;
	}

	void Sketch::deleteRelation(int id) {

		mRelations.erase(id);

	}

	void Sketch::clearRelations() {

		mRelations.clear();

	}

	size_t Sketch::numRelations() {

		return mRelations.size();
	}

	Point* Sketch::addPoint(glm::vec3 point) {

		Point pointToAdd;

		int16_t pointSketchId = -1;

		// Already points in sketch
		/*if (Points.size() > 0) {

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
				pointToAdd.Type = feat_point;

				Points[featureCounter] = pointToAdd;

				if (&Points[featureCounter] == nullptr)
					throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

				featureCounter++;

				return &Points[featureCounter - 1];

			}
		}
		// No points in sketch
		else {

			pointToAdd.pos = point;
			pointToAdd.setId(featureCounter);
			pointToAdd.Type = feat_point;

			Points[featureCounter] = pointToAdd;

			if (&Points[featureCounter] == nullptr)
				throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

			featureCounter++;

			return &Points[featureCounter - 1];

		}
		*/

		pointToAdd.pos = point;
		pointToAdd.setId(featureCounter);
		pointToAdd.Type = feat_point;

		Points[featureCounter] = pointToAdd;

		if (&Points[featureCounter] == nullptr)
			throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

		featureCounter++;

		return &Points[featureCounter - 1];

	}

	void Sketch::deletion(std::vector<int> ids) {

		for (const auto& id : ids) {
			
			Points.erase(id);
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