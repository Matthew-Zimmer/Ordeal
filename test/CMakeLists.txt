Target_Sources(test PUBLIC Main.cpp Tests.cpp)
Target_Link_Libraries(test PUBLIC ordeal)
Target_Compile_Features(test PUBLIC cxx_std_17)