project(gecode-composition_management)
cmake_minimum_required(VERSION 2.8)

target_link_libraries(gecode-test gecodegist gecodedriver gecodeminimodel gecodesearch gecodeint gecodekernel gecodesupport)


add_subdirectory(src)