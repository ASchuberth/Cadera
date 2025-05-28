#include "pch.hpp"
#include "observer.hpp"

namespace CADERA_APP_NAMESPACE {

Observer::Observer() {}

Observer::~Observer() {}

void Observer::onNotify(int id, const RenderData& renderables)
{
    std::cout << renderables << std::endl;
}

}