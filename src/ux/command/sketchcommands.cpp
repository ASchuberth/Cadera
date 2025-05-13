#include "sketchcommands.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

// Sketch Add Point Command
//-----------------------------------------------------------------------------
SketchAddPointCommand::SketchAddPointCommand() : mSketch{nullptr}, 
                                                 mSelector{nullptr},
                                                 mCamera{nullptr}
 {}


void SketchAddPointCommand::setSketch(sketch::Sketch *S) { mSketch = S; }


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

        mSketch->notify();


    }
}
//-----------------------------------------------------------------------------

// Sketch Move Point Command
//-----------------------------------------------------------------------------
SketchMovePointCommand::SketchMovePointCommand() : mSketch{nullptr}, 
                                                 mSelector{nullptr},
                                                 mCamera{nullptr}
 {}

void SketchMovePointCommand::setSketch(sketch::Sketch *S) { mSketch = S; }

void SketchMovePointCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchMovePointCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void SketchMovePointCommand::execute() {
    if (mSketch && mSelector && mCamera) {

        mSelector->select(mCamera->mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                          mCamera->cameraVec, mCamera->pos,
                          mCamera->cross,
                          mCamera->flags.test(cad::cam::ortho));


        mSketch->movePoints(mSelector->selectedPoints, mSelector->point,
            mSelector->flags.test(cad::sel::select_first_click));

        std::vector<int> ids = mSelector->getSelectedPointIds();

        std::map<int, Point> newPoints;

        for (const auto &id : ids) {
        newPoints[id] = mSketch->Points[id];
        }

        mSelector->update(newPoints);

        mSelector->flags.reset(cad::sel::select_first_click);

        mSketch->notify();

    }
}
//-----------------------------------------------------------------------------


// Sketch Select Point Command
//-----------------------------------------------------------------------------
SketchSelectPointCommand::SketchSelectPointCommand() : mSketch{nullptr}, 
                                                       mSelector{nullptr},
                                                       mCamera{nullptr} {}

void SketchSelectPointCommand::setSketch(sketch::Sketch *S) { mSketch = S; }

void SketchSelectPointCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchSelectPointCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void SketchSelectPointCommand::execute() {
    if (mSketch && mSelector && mCamera) {

        mSelector->select(mCamera->mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                        mCamera->cameraVec, mCamera->pos,
                        mCamera->cross,
                        mCamera->flags.test(cad::cam::ortho));

        int id = mSelector->add(mSelector->point, mSketch->Points,
                                    mCamera->camDistance);

        if (id >= 0 || mSelector->selectedPoints.empty()) {
            mSketch->notify();
        }

        mSketch->flags.set(cad::sketch::skt_move_points);

    }
}
//-----------------------------------------------------------------------------

// Sketch Deselect Point Command
//-----------------------------------------------------------------------------
SketchDeselectPointCommand::SketchDeselectPointCommand() : mSketch{nullptr}, 
                                                       mSelector{nullptr},
                                                       mCamera{nullptr} {}

void SketchDeselectPointCommand::setSketch(sketch::Sketch *S) { mSketch = S; }

void SketchDeselectPointCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchDeselectPointCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void SketchDeselectPointCommand::execute() {

    if (mSketch && mSelector && mCamera) {
       
        if (!mSketch->flags.test(cad::sketch::skt_points_have_moved)) {

            mSelector->select(mCamera->mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                              mCamera->cameraVec, mCamera->pos,
                              mCamera->cross,
                              mCamera->flags.test(cad::cam::ortho));

            int id = mSelector->remove(mSelector->point, mSketch->Points,
                                            mCamera->camDistance);

            if (id >= 0 || mSelector->selectedPoints.empty()) {
                mSketch->notify();
            }
        }
    

        mSelector->flags.set(cad::sel::select_first_click);
        mSketch->flags.reset(cad::sketch::skt_move_points);
        mSketch->flags.reset(cad::sketch::skt_points_have_moved);

    }

  
}
//-----------------------------------------------------------------------------

// Sketch Select Set Ctrl Command
//-----------------------------------------------------------------------------
SketchSelectSetCtrlCommand::SketchSelectSetCtrlCommand() : mSelector{nullptr}
{
}

void SketchSelectSetCtrlCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchSelectSetCtrlCommand::execute() {
    if (mSelector)
        mSelector->flags.set(sel::select_isCTRL);
}


//-----------------------------------------------------------------------------

// Sketch Select Unset Ctrl Command
//-----------------------------------------------------------------------------
SketchSelectUnsetCtrlCommand::SketchSelectUnsetCtrlCommand() : mSelector{nullptr}
{
}

void SketchSelectUnsetCtrlCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchSelectUnsetCtrlCommand::execute() {
    if (mSelector)
        mSelector->flags.reset(sel::select_isCTRL);
}


//-----------------------------------------------------------------------------

// Sketch Disable Tools Command
//-----------------------------------------------------------------------------
SketchDisableToolsCommand::SketchDisableToolsCommand() : mSketch{nullptr}  {}

void SketchDisableToolsCommand::setSketch(sketch::Sketch *S) {
    mSketch = S;
}

void SketchDisableToolsCommand::execute() {
    if (mSketch) {
        mSketch->deactivateTools();
    }
}
//-----------------------------------------------------------------------------

// Sketch Add Point Command
//-----------------------------------------------------------------------------
SketchDeleteCommand::SketchDeleteCommand() : mSketch{nullptr},
                                             mSelector{nullptr} {}

void SketchDeleteCommand::setSketch(sketch::Sketch *S) { mSketch = S; }

void SketchDeleteCommand::setSelector(sel::Selector *sel) {
    mSelector = sel;
}

void SketchDeleteCommand::execute() {
    if (mSketch && mSelector) {

        if (!mSelector->getSelectedPointIds().empty()) {

            mSketch->deletion(mSelector->getSelectedPointIds());
            mSelector->clear();
            mSelector->setFlags();

            mSketch->notify();
        }
    }
}

//-----------------------------------------------------------------------------
}
}