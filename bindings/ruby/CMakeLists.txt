include(RockRuby)
include_directories(${CMAKE_BINARY_DIR}/include)
rock_find_pkgconfig(BASE_TYPES REQUIRED base-lib)
rock_ruby_rice_extension(constrained_based_networks_ruby ext/ConstrainedBasedNetworks.cpp)
target_link_libraries(constrained_based_networks_ruby constrained_based_networks)
set(FILES constrained_based_networks.rb
    constrained_based_networks/core.rb
    )

rock_ruby_library(constrained_based_networks ${FILES})

