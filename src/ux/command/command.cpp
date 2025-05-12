#include "command.hpp"

namespace CADERA_APP_NAMESPACE {
namespace command {

Command::Command() {}

Command::~Command() {}

void Command::execute() { std::cout << "Command Executed!" << std::endl; }

void Command::execute(double d) {};


} // namespace action
} // namespace CADERA_APP_NAMESPACE
