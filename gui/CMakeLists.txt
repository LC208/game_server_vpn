find_package(Qt6)
find_package(libb2 REQUIRED)
add_executable(${PROGRAM_NAME} main.cpp)

target_link_libraries(${PROGRAM_NAME}
    qt::qt
    libb2::libb2
    ${PROGRAM_NAME}_lib
)