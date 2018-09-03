set(libutils_SRCS
utils.h
utils.cpp
)

# Declare the library
add_library(libutils STATIC
${libutils_SRCS}
)

# Specify here the include directories exported
# by this library
target_include_directories(libutils PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)
