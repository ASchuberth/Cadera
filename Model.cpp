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

	void Model::setType(ModelType type) {

		mType = type;
	}

}