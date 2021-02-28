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
		featureCounter = 1;

		mCamDistance = nullptr;
	}

	int Sketch::getFeatureCounter() {

		return featureCounter;
	}

	void Sketch::setCameraDistance(float* camDistance) {
		mCamDistance = camDistance;
	}

	void Sketch::activatePointTool() {
		deactivateTools();
		flags.set(skt_point_tool);
		flags.set(skt_tool_active);
	}

	void Sketch::activateNoteTool() {
		deactivateTools();
		flags.set(skt_note_tool);
		flags.set(skt_tool_active);
	}

	void Sketch::deactivateTools() {
		
		flags.reset();
	}

	void Sketch::add(glm::vec3 point) {

		if (flags.test(skt_point_tool)) {
			addPoint(point);
		}

		if (flags.test(skt_note_tool)) {
			addNotePoint(point);
		}


	}



	void Sketch::addRelation(const std::vector<int>& ids, RelationType Type) {

		Relation newRelation;

		newRelation.mId = featureCounter;

		for (const auto& id : ids) {
			newRelation.mFeatureIds.push_back(id);

			Points[id].relationIds.push_back(newRelation.mId);
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


		pointToAdd.pos = point;
		pointToAdd.setId(featureCounter);
		pointToAdd.Type = feat_point;

		Points[featureCounter] = pointToAdd;

		if (&Points[featureCounter] == nullptr)
			throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

		featureCounter++;

		return &Points[featureCounter - 1];

	}

	Point* Sketch::addConstructionPoint(glm::vec3 point) {

		Point pointToAdd;

		int16_t pointSketchId = -1;


		pointToAdd.pos = point;
		pointToAdd.setId(featureCounter);
		pointToAdd.Type = feat_construction;

		Points[featureCounter] = pointToAdd;

		if (&Points[featureCounter] == nullptr)
			throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

		featureCounter++;

		return &Points[featureCounter - 1];
	}

	Point* Sketch::addNotePoint(glm::vec3 point) {

		Point pointToAdd;

		int16_t pointSketchId = -1;


		pointToAdd.pos = point;
		pointToAdd.setId(featureCounter);
		pointToAdd.Type = feat_note;

		Points[featureCounter] = pointToAdd;

		if (&Points[featureCounter] == nullptr)
			throw std::runtime_error("Sketch.addPoint(): Funtion returned a nullptr!");

		featureCounter++;

		pcs::txt::Text T;

		T.cursorPos = point;
		T.cursorDirX = { 0.0f, 0.0f, 1.0f };
		T.cursorDirY = { 0.0f, 1.0f, 0.0f };
		T.backgroundColor = { 0.1f, 0.1f, 0.1f };
		T.textColor = { 1.0f, 1.0f, 1.0f };
		T.text = text;

		Notes[featureCounter] = T;
		featureCounter++;

		return &Points[featureCounter - 2];
	}

	void Sketch::deletion(std::vector<int> ids) {

		for (const auto& id : ids) {
			
			Points.erase(id);
		}
	}

	std::vector<glm::vec3> Sketch::getVertices(std::vector<glm::vec3>& colors) {

		std::vector<glm::vec3> vertices;
		colors.clear();

		for (const auto& p : Points) {
			vertices.push_back(p.second.pos);

			if (p.second.Type == feat_point) {
				colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			}
			else if (p.second.Type == feat_construction) {
				colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
			}
			else if (p.second.Type == feat_note) {
				colors.push_back(glm::vec3(1.0f, 0.8f, 0.0f));
			}
		}

		return vertices;
	}

}
}