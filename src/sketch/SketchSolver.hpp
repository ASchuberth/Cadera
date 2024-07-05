#pragma once
#include "Sketch.hpp"

namespace CADERA_APP_NAMESPACE {




	template<typename T>
	inline std::vector<T> vectorSameElements(std::vector<T> v1, std::vector<T> v2) {

		std::vector<T> SameElements;

		for (const auto& elem : v1) {
			
			bool isSame = std::find(v2.begin(), v2.end(), elem) != v2.end();

			if (isSame)
				SameElements.push_back(elem);
	
		}

		return SameElements;
	}

namespace sketch {



	class SketchSolver {

	public:

		void setActiveSketch(Sketch* S);

		bool checkNewRelation(const Relation& R);

		bool checkCommenRelationPts(const Relation& R);

		std::list<int> getRelatedPointIds(const int& Pid);

		bool addNewSketchRelation(const std::vector<int>& ids, RelationType Type);

		

	private:

		Sketch* pActiveSketch = nullptr;


	};

}
}

