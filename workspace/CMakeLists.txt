cmake_minimum_required(VERSION 3.12)

project(KasaFiskalnaPoprzezTDD CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

add_library(kasa
  src/Kasa.cpp
)
target_include_directories(kasa PUBLIC include)
target_compile_features(kasa PUBLIC cxx_std_23)

add_executable(kasa_tests 
  src/KasaTests.cpp
)
target_include_directories(kasa_tests PUBLIC include)
target_compile_features(kasa_tests PUBLIC cxx_std_23)
target_link_libraries(kasa_tests gmock_main gtest gmock kasa)

add_custom_target(run_tests
                  COMMAND kasa_tests
                  DEPENDS kasa_tests
                  USES_TERMINAL)

enable_testing()
add_test(tests kasa_tests)

