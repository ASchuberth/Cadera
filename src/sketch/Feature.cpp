#include "feature.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

Feature::Feature() { mId = -1; }

Feature::Feature(int id) { mId = id; }

void Feature::setId(int id) { mId = id; }

int Feature::getId() { return mId; }

} // namespace CADERA_APP_NAMESPACE