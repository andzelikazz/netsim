cmake_minimum_required(VERSION 3.24)
project(netsim)

set(CMAKE_CXX_STANDARD 17)


set(SOURCE_DIR src)
set(SOURCE_FILES
    ${SOURCE_DIR}/package.cpp
    ${SOURCE_DIR}/helpers.cpp
    ${SOURCE_DIR}/nodes.cpp
    ${SOURCE_DIR}/factory.cpp
    ${SOURCE_DIR}/storage_types.cpp
    ${SOURCE_DIR}/simulation.cpp
)

add_executable(netsim main.cpp ${SOURCE_FILES})
target_include_directories(netsim PUBLIC src include)

add_subdirectory(googletest-1.17.0 googletest-master)

set(TEST_FILES
    tests/main_gtest.cpp
    tests/test_products.cpp
    tests/test_nodes.cpp
    tests/test_factory.cpp
    tests/test_factory_io.cpp
    tests/test_simulation.cpp
)

add_executable(netsim_tests ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(netsim_tests PUBLIC src include)
target_link_libraries(netsim_tests gtest gmock)

enable_testing()
add_test(NAME AllTests COMMAND netsim_tests)