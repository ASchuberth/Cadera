#pragma once

#include "ux/command/input.hpp"

namespace CADERA_APP_NAMESPACE {
namespace gui {

enum GUIFlags {
  gui_start_menu,
  gui_sketch_menu,
  gui_sketch_grid_menu,
  gui_num_flags
};

void imguiRun(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel
              );

void imguiBegin();

void imguiEnd();

void mainMenuBar();

void startMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel,
               std::bitset<gui_num_flags> &flags);

void sketchMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel,
                std::bitset<gui_num_flags> &flags);

void gridMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel,
              
              std::bitset<gui_num_flags> &flags);

void showDebugWindow(sketch::Sketch &Sketch, CADRender &Render,
                     sel::Selector &Sel,
                     std::bitset<gui_num_flags> &flags);


} // namespace gui
} // namespace CADERA_APP_NAMESPACE