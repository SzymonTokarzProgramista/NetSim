cmake_minimum_required(VERSION 3.21)
project(netsim)

set(CMAKE_CXX_STANDARD 17)

# Dodaj flagi kompilacji wymuszające pisanie kodu zgodnego ze standardem.
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        #src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        src/types.cpp
        )

#add_executable(netsim ${SOURCE_FILES} main.cpp)

# == Unit testing using Google Testing Framework ==

# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.

#                      UWAGA WAŻNE !!!
#  JAK COŚ NIE JEST ZAIMPLEMENTOWANE TO MUSI BYĆ ZAKOMENTOWANE !!!
set(SOURCES_FILES_TESTS
        #test/test_Factory.cpp
        #test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        #test/test_reports.cpp
        #test/test_simulate.cpp
        test/test_storage_types.cpp
        )

# Dodaj konfigurację typu `Test`.
add_executable(netsim ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
#target_compile_definitions(netsim PUBLIC EXERCISE_ID = 3)

# Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
target_include_directories(netsim PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master)
target_link_libraries(netsim gmock)
