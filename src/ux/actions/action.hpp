#pragma once

#include "sketch/SketchSolver.hpp"

namespace CADERA_APP_NAMESPACE {

namespace action {


    enum ActionType {
        sketch_add,
        sketch_delete,
        number_action_types
    };


    class Action {


        
        //ModelType m_ModelType;
    
    public:
        
        ActionType m_ActionType;
        
        Action();
        ~Action();

        virtual void submit();
    
    };
}
}