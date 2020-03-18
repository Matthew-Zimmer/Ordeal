target_sources(ordeal PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/main.cpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ordeal.cpp>)