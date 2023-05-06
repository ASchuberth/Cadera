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

		inline void removeId(int id) {
			
			std::list<int>::iterator iter = mFeatureIds.begin();

			while (iter != mFeatureIds.end()) {
				
				if (*iter == id) {
					iter = mFeatureIds.erase(iter);
				}
				else {
					++iter;
				}
			}

		}
	};

}
