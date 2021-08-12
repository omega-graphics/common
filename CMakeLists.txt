cmake_minimum_required(VERSION 3.9)

project(OmegaCommon C CXX)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(OmegaGraphicsSuite)

set(COMMON_SRCS "src/crt.c" "src/format.cpp" "src/json.cpp" "src/fs.cpp" "src/utils.cpp" "src/xml.cpp")

add_omega_graphics_module("OmegaCommon" STATIC SOURCES ${COMMON_SRCS} HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
if(WIN32)
	target_sources("OmegaCommon" PRIVATE "src/fs-win.cpp"  "src/net-win.cpp" "src/multithread-win.cpp")
elseif(APPLE)
	target_sources("OmegaCommon" PRIVATE "src/fs-cocoa.mm"  "src/net-curl.cpp")
endif()

file(GLOB WRAPGEN_SRCS "wrapgen/*.cpp")

add_omega_graphics_tool("omega-wrapgen" LIBS "OmegaCommon" SOURCES ${WRAPGEN_SRCS} "wrapgen/main/main.cpp")

file(GLOB EBIN_SRCS "embedbin/*.cpp")

add_omega_graphics_tool("omega-ebin" LIBS "OmegaCommon" SOURCES ${EBIN_SRCS})


