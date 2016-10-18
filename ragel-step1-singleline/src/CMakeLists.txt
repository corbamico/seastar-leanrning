set(SEASTAR_HOME "/home/lilas/Project/github/seastar/build/debug")
set(ENV{PKG_CONFIG_PATH} ${SEASTAR_HOME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)


find_package(PkgConfig)
#pkg_check_modules(SEASTAR REQUIRED seastar)

#include_directories(${SEASTAR_INCLUDE_DIRS})
#link_directories(${SEASTAR_LIBRARY_DIRS})
#add_definitions(${SEASTAR_CFLAGS_OTHER})


find_package(RAGEL 6.8  REQUIRED)
RAGEL_TARGET(ragel-step1 
             main.rl 
             ${CMAKE_CURRENT_BINARY_DIR}/main.cpp
             COMPILE_FLAGS "-C -G2")

add_executable(ragel-step1 main.cpp)


#target_link_libraries(ragel-step1 seastar pthread ${SEASTAR_LIBRARIES})
