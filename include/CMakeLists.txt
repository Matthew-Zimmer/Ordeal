Target_Include_Directories(ordeal INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/ordeal>)

#TODO: Fill in .hpp Files
Set(Headers 
	Testing.hpp)

Install(FILES ${Headers} DESTINATION include/ordeal)