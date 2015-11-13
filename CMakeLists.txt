# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_activate_cxx11()
rock_init(constrained_based_networks 0.1)
rock_find_cmake(Boost COMPONENTS serialization system REQUIRED)
rock_standard_layout()
file(GLOB BIN_FILES "${PROJECT_SOURCE_DIR}/bin/*")
INSTALL(FILES ${BIN_FILES} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE DESTINATION bin/)
#add_subdirectory(tests)
