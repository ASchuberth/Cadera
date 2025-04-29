#pragma once

#include "action.hpp"

namespace CADERA_APP_NAMESPACE {

namespace action {

class ActionQueue {

public:
  ActionQueue();
  ~ActionQueue();

  std::list<Action> Actions;

  void poll();

  void add(Action &Act);
};
} // namespace action
} // namespace CADERA_APP_NAMESPACE