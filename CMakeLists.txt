cmake_minimum_required (VERSION 3.8)
project ("Ants")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Dodaj źródło do pliku wykonywalnego tego projektu.

add_executable (Ants 
"AntsSimulator/display.cpp" 
"AntsSimulator/controls.cpp" 
"AntsSimulator/shaders.cpp" 
"AntsSimulator/texture_load.h" 
"AntsSimulator/texture_load.cpp"
"AntsSimulator/ant.cpp"
"AntsSimulator/ant.h"
"AntsSimulator/food.cpp"
"AntsSimulator/food.h"
"AntsSimulator/nest.cpp"
"AntsSimulator/nest.h"
"AntsSimulator/trace.cpp"
"AntsSimulator/trace.h")

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
target_link_libraries(Ants ${CONAN_LIBS})
