set(SEASTAR_HOME "/home/lilas/Project/github/seastar/build/debug")
set(ENV{PKG_CONFIG_PATH} ${SEASTAR_HOME})


find_package(PkgConfig)
pkg_check_modules(SEASTAR REQUIRED seastar)



include_directories(${SEASTAR_INCLUDE_DIRS})
link_directories(${SEASTAR_LIBRARY_DIRS})
add_definitions(${SEASTAR_CFLAGS_OTHER})

add_executable(echo main.cpp echo.cpp)
target_link_libraries(echo seastar pthread ${SEASTAR_LIBRARIES})