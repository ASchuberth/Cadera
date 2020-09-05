#include "pch.hpp"
#include "Sketch.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {


	Sketch::Sketch() {
		
	}

	Sketch::Sketch(int i) {
		setId(i);
		setType(cad_sketch);
	}

}
}