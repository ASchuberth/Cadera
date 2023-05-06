#include "pch.hpp"
#include "Model.hpp"

namespace CADERA_APP_NAMESPACE {
	
	Model::Model() {

	}

	
	Model::Model(int id) {
		mId = id;
	}

	void Model::setId(int id) {
		
		mId = id;

	}

	int Model::getId() {
		return mId;
	}

	void Model::setType(ModelType type) {

		mType = type;
	}

	ModelType Model::getType() {

		return mType;
	}

	std::vector<txt::Text> Model::getRelationTexts()
	{
		return std::vector<txt::Text>();
	}

	std::vector<glm::vec3> Model::getVertices() {

		return std::vector<glm::vec3>();
	}

	std::vector<glm::vec3> Model::getVertices(std::vector<glm::vec3>& colors)
	{
		return std::vector<glm::vec3>();
	}

}