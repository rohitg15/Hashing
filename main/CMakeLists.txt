
set(Main_SRCS
main.cpp
)

add_executable(hashbreaker
${Main_SRCS}
)

# Specify here the libraries this program depends on
target_link_libraries(hashbreaker
libmurmur
libcollisions
)

install(TARGETS hashbreaker DESTINATION bin)