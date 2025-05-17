#pragma once
#include "ux/gui.hpp"
#include "callbacks.hpp"

namespace CADERA_APP_NAMESPACE {

/// @brief Flags that can be toggled in a bitset to trigger events
enum CaderaFlags {

  /// Will trigger deletion of the currently selected object
  cadera_delete,
  /// Number of flags to be used in the creation of the bitset
  cadera_num_flags
};

/**
 * @brief Cadera
 * @author Austin Schuberth
 * @page Cadera
 *
 */
class Cadera {

private:
  void initCallbacks();

  std::map<int, Model> Models;

  int modelIdCounter;

  sel::Selector mSelector;
  CADRender mRender;

  sketch::Sketch mSketch;

public:
  // TODO: Restructure
  command::SketchAddPointCommand sketchAddPointCmd;
  command::SketchMovePointCommand sketchMovePointCmd;
  command::SketchSelectPointCommand sketchSelectPointCmd;
  command::SketchSelectSetCtrlCommand sketchSelectSetCtrlCmd;
  command::SketchSelectUnsetCtrlCommand sketchSelectUnsetCtrlCmd;
  command::SketchDeselectPointCommand sketchDeselectPointCmd;
  command::SketchDisableToolsCommand sketchDisableToolsCmd;
  command::SketchDeleteCommand sketchDeleteCmd;

  command::CameraZoomCommand cameraZoomCmd;
  command::CameraPanCommand cameraPanCmd;
  command::CameraUnsetPanCommand cameraUnsetPanCmd;

  command::RenderFramebufferResizeCommand renderFramebufferResizeCmd;


  command::Input input;
  //-------------

  
  
  

  std::bitset<cadera_num_flags> flags;

  Cadera();

  ~Cadera();

  void SketchEvents();

  void run();

  void mainLoop();
};

} // namespace CADERA_APP_NAMESPACE
