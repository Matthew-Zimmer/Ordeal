CMake_Minimum_Required(VERSION 3.13)

Project(ordeal)

Add_Library(ordeal INTERFACE)
Target_Compile_Features(ordeal INTERFACE cxx_std_17)
Add_Executable(test "")

Add_Subdirectory(src)
Add_Subdirectory(include)
Add_Subdirectory(tests)

Install(TARGETS ordeal EXPORT ordeal DESTINATION lib/ordeal)
Install(EXPORT ordeal DESTINATION lib/ordeal)
Install(FILES ordeal-config.cmake DESTINATION lib/ordeal)