#pragma once
#include "TextRender.hpp"

namespace CADERA_APP_NAMESPACE {
	
	enum ModelType {
		cad_sketch,
		cad_layout,
		cad_part,
		cad_assembly,
		cad_drawing
	};
	
	class Model {

		int mId;
		ModelType mType;

	public:

		Model();
		Model(int id);

		void setId(int id);

		int getId();

		void setType(ModelType type);

		ModelType getType();

		std::map<int, txt::Text> Notes;
		
		virtual std::vector<txt::Text> getRelationTexts();

		virtual std::vector<glm::vec3> getVertices();
		virtual std::vector<glm::vec3> getVertices(std::vector<glm::vec3>& colors);
		
	};

}

