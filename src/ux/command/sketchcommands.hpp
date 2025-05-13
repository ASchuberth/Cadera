#pragma once
#include "command.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class SketchAddPointCommand : public Command { 
public:

    SketchAddPointCommand();

    void setSketch(sketch::Sketch* S);
    void setSelector(sel::Selector* sel);
    void setCamera(cam::Camera* cam);

    void execute() override;

private:

    sketch::Sketch* mSketch;
    sel::Selector* mSelector;
    cam::Camera* mCamera;

};

class SketchMovePointCommand : public Command { 
    public:
    
        SketchMovePointCommand();
    
        void setSketch(sketch::Sketch* S);
        void setSelector(sel::Selector* sel);
        void setCamera(cam::Camera* cam);
    
        void execute() override;
    
    private:
    
        sketch::Sketch* mSketch;
        sel::Selector* mSelector;
        cam::Camera* mCamera;
    
    };

class SketchSelectPointCommand : public Command {
public:

    SketchSelectPointCommand();

    void setSketch(sketch::Sketch* S);
    void setSelector(sel::Selector* sel);
    void setCamera(cam::Camera* cam);

    void execute() override;

private:

    sketch::Sketch* mSketch;
    sel::Selector* mSelector;
    cam::Camera* mCamera;

};

class SketchDeselectPointCommand : public Command {
    public:
    
        SketchDeselectPointCommand();
    
        void setSketch(sketch::Sketch* S);
        void setSelector(sel::Selector* sel);
        void setCamera(cam::Camera* cam);
    
        void execute() override;
    
    private:
    
        sketch::Sketch* mSketch;
        sel::Selector* mSelector;
        cam::Camera* mCamera;
    
};

class SketchDisableToolsCommand : public Command { 
public:

    SketchDisableToolsCommand();

    void setSketch(sketch::Sketch* S);

    void execute() override;

private:

    sketch::Sketch* mSketch;

};

class SketchDeleteCommand : public Command { 
    public:
    
        SketchDeleteCommand();
    
        void setSketch(sketch::Sketch* S);
        void setSelector(sel::Selector* sel);
    
        void execute() override;
    
    private:
    
        sketch::Sketch* mSketch;
        sel::Selector* mSelector;
    
    };

}
}