file(GLOB ROOT_SOURCE *.cpp)

file(GLOB_RECURSE GUI_SOURCE ./Gui/*.cpp)
file(GLOB_RECURSE GUI_HEADER ./Gui/*.h)

file(GLOB_RECURSE GAME_SOURCE ./Game/*.cpp)
file(GLOB_RECURSE GAME_HEADER ./Game/*.h)

file(GLOB_RECURSE UTILITY_SOURCE ./Utility/*.cpp)
file(GLOB_RECURSE UTILITY_HEADER ./Utility/*.h)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


add_executable(Orbital ${ROOT_SOURCE} ${GUI_SOURCE} ${GUI_HEADER} ${GAME_SOURCE} ${GAME_HEADER} ${UTILITY_SOURCE} ${UTILITY_HEADER})

target_link_libraries(Orbital CONAN_PKG::eigen CONAN_PKG::box2d)
include_directories("./")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Orbital sfml-graphics sfml-window sfml-audio)
else()
    message(FATAL_ERROR "Failed to find SFML")
endif()
