target_include_directories(ordeal PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/ordeal>)

target_include_directories(ordeal_test PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

set(headers 
	ordeal.hpp)

install(FILES ${headers} DESTINATION include/ordeal)