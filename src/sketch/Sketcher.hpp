#pragma once
#include "Sketch.hpp"




namespace CADERA_APP_NAMESPACE {
namespace sketch {
	


	class Sketcher {

		Sketcher();

		~Sketcher();

		std::unique_ptr<Sketch> activeSketch;
		

		std::bitset<skt_num_flags> flags;

		void add(glm::vec3 pointToAdd);



	};

}
}

