#include "subject.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

Subject::Subject()
{
}

Subject::~Subject() {}

void Subject::addRender(Observer *observer) {

    observers.push_back(observer);

}

void Subject::removeRender(Observer *observer) {

}

void Subject::notify() {

    for (const auto& observer : observers) {
        std::cout << "Subject notifies." << std::endl;
        observer->onNotify(int {0}, RenderItems{});
    }

}

}