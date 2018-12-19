add_definitions(-DLIBMSGDIST_BUILD)
file(GLOB_RECURSE LIB_SRC "*.h" "*.hpp" "*.cpp")
add_library(udp STATIC ${LIB_SRC})

