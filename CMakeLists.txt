cmake_minimum_required (VERSION 3.8)
project ("Ants")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (Ants "display.cpp" "objects.cpp" "controls.cpp" "shaders.cpp" "texture_load.h" "texture_load.cpp")

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
target_link_libraries(Ants ${CONAN_LIBS})