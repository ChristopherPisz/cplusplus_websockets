cmake_minimum_required(VERSION 3.0.0)
project(boost_beast_websocket_server_poc VERSION 0.1.0)

include(CTest)
enable_testing()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(BOOST_ROOT "~/git-workspace/boost_1_72_0/")
find_package(Boost 1.72.0 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

find_package(Threads REQUIRED)

include(boost_beast_websocket_server_poc/CMakeLists.txt)
add_executable(boost_beast_websocket_server_poc ${MyApp_SOURCES})
target_link_libraries(boost_beast_websocket_server_poc ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(boost_beast_websocket_server_poc PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )