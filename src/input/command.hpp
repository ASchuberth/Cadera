#pragma once

#include "render/cadrender.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class Command {



public:


  Command();
  ~Command();

  virtual void execute();

  virtual void execute(double);
};
} // namespace action
} // namespace CADERA_APP_NAMESPACE