set(libcollisions_SRCS
murmurcollisions.h
murmurcollisions.cpp
)

# Declare the library
add_library(libcollisions STATIC
${libcollisions_SRCS}
)

target_link_libraries(
    libcollisions
    libmurmur
    libutils
)


# Specify here the include directories exported
# by this library
target_include_directories(libcollisions PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
)
