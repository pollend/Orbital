file(GLOB ROOT_SOURCE *.cpp)

file(GLOB_RECURSE GUI_SOURCE ./Gui/*.cpp)
file(GLOB_RECURSE GUI_HEADER ./Gui/*.h)

file(GLOB_RECURSE GAME_SOURCE ./Game/*.cpp)
file(GLOB_RECURSE GAME_HEADER ./Game/*.h)

file(GLOB_RECURSE OTHER_SOURCE ./ProjectOrbital/*.cpp)
file(GLOB_RECURSE OTHER_HEADER ./ProjectOrbital/*.h)


include_directories("./")
add_executable(Orbital ${ROOT_SOURCE} ${GUI_SOURCE} ${GUI_HEADER} ${GAME_SOURCE} ${GAME_HEADER} ${OTHER_SOURCE} ${OTHER_HEADER})
target_link_libraries(Orbital CONAN_PKG::sfml CONAN_PKG::eigen)