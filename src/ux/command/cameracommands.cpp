#include "cameracommands.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {



CameraZoomCommand::CameraZoomCommand() : mCamera{nullptr} {}


void CameraZoomCommand::setCamera(cam::Camera *cam) {
    mCamera = cam;
}

void CameraZoomCommand::execute(double yoffset) {
    if (mCamera) {

        mCamera->zoom(static_cast<float>(yoffset));

    }
}

}
}