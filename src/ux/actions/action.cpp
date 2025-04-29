#include "action.hpp"

namespace CADERA_APP_NAMESPACE {
namespace action {

Action::Action() {}

Action::~Action() {}

void Action::submit() { std::cout << "Action Submitted" << std::endl; }
} // namespace action
} // namespace CADERA_APP_NAMESPACE
