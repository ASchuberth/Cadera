#pragma once


namespace CADERA_APP_NAMESPACE {

	enum RelationType {
		rel_vertical,
		rel_horizontal,
		rel_coincident,
	};

	struct Relation {

		int mId;
		std::list<int> mFeatureIds;
		RelationType mType;


	};

}
