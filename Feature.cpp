#include "pch.hpp"
#include "Feature.hpp"

namespace CADERA_APP_NAMESPACE {
	
	Feature::Feature() {

	}

	Feature::Feature(int id) {
		mId = id;
	}

	void Feature::setId(int id) {
		mId = id;
	}

	int Feature::getId() {
		return mId;
	}
	
}