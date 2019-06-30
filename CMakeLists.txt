CMake_Minimum_Required(VERSION 3.13)

Project(ordeal)

Add_Library(ordeal INTERFACE)
Target_Compile_Features(ordeal INTERFACE cxx_std_17)
Add_Executable(test "")

Add_Subdirectory(Source)
Add_Subdirectory(Include)
Add_Subdirectory(Imp)
Add_Subdirectory(Test)

Install(TARGETS ordeal EXPORT ordeal DESTINATION lib/ordeal)
Install(EXPORT ordeal DESTINATION lib/ordeal)
Install(FILES ordeal-config.cmake DESTINATION lib/ordeal)

Set(CMAKE_CXX_STANDARD "libc++")