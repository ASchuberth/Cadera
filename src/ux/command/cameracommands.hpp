#pragma once
#include "sketchcommands.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class CameraZoomCommand : public Command { 
public:

    CameraZoomCommand();

    void setCamera(cam::Camera* cam);

    void execute(double) override;

private:
    cam::Camera* mCamera;

};

}
}