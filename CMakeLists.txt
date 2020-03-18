cmake_minimum_required(VERSION 3.13)

project(ordeal)

add_library(ordeal)
Add_executable(ordeal_test "")

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(test)

target_compile_features(ordeal PUBLIC cxx_std_17)
target_compile_features(ordeal_test PUBLIC cxx_std_17)

set(CMAKE_CXX_FLAGS "-fmax-errors=1")

install(TARGETS ordeal EXPORT ordeal DESTINATION lib/ordeal)
install(EXPORT ordeal DESTINATION lib/)
install(FILES ordeal-config.cmake DESTINATION lib/ordeal)