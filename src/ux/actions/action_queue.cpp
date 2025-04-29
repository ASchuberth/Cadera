#include "action_queue.hpp"


namespace CADERA_APP_NAMESPACE {
    namespace action {

        ActionQueue::ActionQueue() {

        }

        ActionQueue::~ActionQueue() {

        }

        void ActionQueue::poll() {

            for (auto & Act : Actions) {

                Act.submit();
            }

            Actions.clear();
        }

        void ActionQueue::add(Action &Act) {

            Actions.push_back(Act);
        }

    } // namespace action
} // namespace CADERA_APP_NAMESPACE
    