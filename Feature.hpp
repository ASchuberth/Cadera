#pragma once
#include "Relation.hpp"

namespace CADERA_APP_NAMESPACE {
	
	enum FeatureType {
		feat_point,
		feat_edge,
		feat_round,
		feat_construction,
		feat_note
	};
	

	class Feature {

		 int mId;


	public:

		Feature();
		Feature(int i);

		FeatureType Type;

		
		

		void setId(int i);

		int getId();

		

	};
}

