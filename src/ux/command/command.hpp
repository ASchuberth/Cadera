#pragma once

#include "render/Camera.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class Command {



public:


  Command();
  ~Command();

  virtual void execute();
};
} // namespace action
} // namespace CADERA_APP_NAMESPACE