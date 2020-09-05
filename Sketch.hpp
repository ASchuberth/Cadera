#pragma once
#include "Feature.hpp"
#include "Model.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {
	
	class Sketch : public Model {

		
		
		
		std::map<int32_t , glm::vec3> Points;

	public:

		Sketch();
		Sketch(int i);
		

	};

}
}
