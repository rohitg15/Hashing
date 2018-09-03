set(libmurmur_SRCS
murmur.h
murmur.cpp
)

# Declare the library
add_library(libmurmur STATIC
${libmurmur_SRCS}
)

# Specify here the include directories exported
# by this library
target_include_directories(libmurmur PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)
