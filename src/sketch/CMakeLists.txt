cmake_minimum_required(VERSION 3.13)



target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Relation.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Feature.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Sketch.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Sketcher.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/SketchSolver.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Point.cpp
                                 )

