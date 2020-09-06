#pragma once
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

		void setType(ModelType type);

		ModelType getType();

		virtual std::vector<glm::vec3> getVertices();
	};

}

