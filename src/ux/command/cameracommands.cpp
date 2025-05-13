#include "cameracommands.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


// Camera Zoom Command
//-----------------------------------------------------------------------------
CameraZoomCommand::CameraZoomCommand() : mCamera{nullptr} {}


void CameraZoomCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void CameraZoomCommand::execute(double yoffset) {
    if (mCamera) {

        mCamera->zoom(static_cast<float>(yoffset));

    }
}
//-----------------------------------------------------------------------------

// Camera Pan Command
//-----------------------------------------------------------------------------
CameraPanCommand::CameraPanCommand() : mCamera{nullptr} {}

void CameraPanCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void CameraPanCommand::execute() {
   if (mCamera) {
    mCamera->flags.set(cad::cam::pan);
    mCamera->flags.set(cad::cam::mouseFirstPressed);
   }
}
//-----------------------------------------------------------------------------

// Camera Unset Pan Command
//-----------------------------------------------------------------------------
CameraUnsetPanCommand::CameraUnsetPanCommand() : mCamera{nullptr} {}

void CameraUnsetPanCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void CameraUnsetPanCommand::execute() {
   if (mCamera) {
    mCamera->flags.reset(cad::cam::pan);
   }
}
//-----------------------------------------------------------------------------

}
}