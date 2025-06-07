find_package(Catch2 REQUIRED)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

add_executable(${TEST_PROGRAM_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_PROGRAM_NAME}
    PRIVATE
        ${PROGRAM_NAME}_lib
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(${TEST_PROGRAM_NAME})