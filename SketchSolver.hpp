#pragma once
#include "Sketch.hpp"

namespace CADERA_APP_NAMESPACE {


	// Returns a vector with any elements in v2 that aren't in v1
	template<typename T>
	inline std::vector<T> getDifferenceVector(std::vector<T> v1, std::vector<T> v2) {

		std::vector<T> differenceVector;

		if (v1.empty())
			return v2;

		for (const auto& elem : v2) {

			bool isFound = std::find(v1.begin(), v1.end(), elem) != v1.end();

			if (!isFound) {

				differenceVector.push_back(elem);

			}

		}

		return differenceVector;

	}

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

		std::vector<int> getRelatedPointIds(const int& Pid);

		bool addNewSketchRelation(const std::vector<int>& ids, RelationType Type);

		

	private:

		Sketch* pActiveSketch = nullptr;


	};

}
}

