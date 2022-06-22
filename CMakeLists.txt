cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(SupercellSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/source/DetectorCopy.cpp)

add_executable(supercellSim main.cpp ${sources} ${headers})
target_link_libraries(supercellSim ${Geant4_LIBRARIES})

add_custom_target(SupercellSim DEPENDS supercellSim)
