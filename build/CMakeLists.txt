cmake_minimum_required (VERSION 2.6)

project (LibRetroServer)

add_subdirectory("${PROJECT_SOURCE_DIR}/../../LibRetroServerCore/build" "${PROJECT_SOURCE_DIR}/../../LibRetroServerCore_build")
include_directories("${PROJECT_SOURCE_DIR}/../../core/inc")
include_directories("${PROJECT_SOURCE_DIR}/../../Platform/inc")
include_directories("${PROJECT_SOURCE_DIR}/../../LibRetroServerCore/inc")

include_directories("${PROJECT_SOURCE_DIR}/../inc/LibRetroServer")

###############################################
#exe
###############################################
file(GLOB_RECURSE lrs_inc "${PROJECT_SOURCE_DIR}/../inc/*.h")
file(GLOB_RECURSE lrs_src1 "${PROJECT_SOURCE_DIR}/../src/*.h")
file(GLOB_RECURSE lrs_src2 "${PROJECT_SOURCE_DIR}/../src/*.cpp")
list(APPEND lrs_src ${lrs_inc})
list(APPEND lrs_src ${lrs_src1})
list(APPEND lrs_src ${lrs_src2})

add_library(LibRetroServer OBJECT ${lrs_src})
						
set_property(TARGET LibRetroServer APPEND PROPERTY COMPILE_FLAGS "-WX -W4")
