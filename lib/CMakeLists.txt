file(GLOB LIB_SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(${PROGRAM_NAME}_lib STATIC ${LIB_SOURCES})

target_include_directories(${PROGRAM_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)