include(RockRuby)
include_directories(${CMAKE_BINARY_DIR}/include)
rock_find_pkgconfig(BASE_TYPES REQUIRED base-lib)
rock_ruby_rice_extension(gecode_composition_management_ruby ext/gecode_composition_management.cpp)
target_link_libraries(gecode_composition_management_ruby gecode_composition_management ${Gecode_LIBRARIES})
set(FILES gecode_composition_management.rb
    gecode_composition_management/core.rb
    )

rock_ruby_library(gecode_composition_management ${FILES})

