project("OmegaCommon" C CXX)

file(GLOB OMEGA_COMMON_HEADERS "./include/omega-common/*.h")

add_library("omega-common" STATIC 
"src/crt.c" 
"src/fs.cpp"
"src/json.cpp"
"src/utils.cpp"
"src/xml.cpp"
${OMEGA_COMMON_HEADERS})

file(GLOB OMEGA_WRAPGEN_SOURCES "./wrapgen/*.cpp" "./wrapgen/*.h" "./wrapgen/*.def")

add_executable("omega-wrapgen" ${OMEGA_WRAPGEN_SOURCES})

