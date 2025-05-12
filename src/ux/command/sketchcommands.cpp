#include "sketchcommands.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {



SketchAddPointCommand::SketchAddPointCommand() : mSketch{nullptr}, 
                                                 mSelector{nullptr},
                                                 mCamera{nullptr}
 {}

void SketchAddPointCommand::setSketch(sketch::Sketch *S) {

    mSketch = S;
}

void SketchAddPointCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchAddPointCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void SketchAddPointCommand::execute() {
    if (mSketch && mSelector && mCamera) {

        mSelector->select(mCamera->mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                          mCamera->cameraVec, mCamera->pos,
                          mCamera->cross,
                          mCamera->flags.test(cad::cam::ortho));

        if (!mSelector->existingPoint(mSelector->point))
            mSketch->add(mSelector->point);


    }
}


}
}