cmake_minimum_required(VERSION 3.16...3.21)

project(Conversion)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(con con.cc ${sources} ${headers})
target_link_libraries(con ${Geant4_LIBRARIES})

add_custom_target(Conversion DEPENDS con)