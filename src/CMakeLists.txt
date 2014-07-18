#find_package(Boost 1.48 COMPONENTS serialization system REQUIRED)

set(SOURCES
   Component.cpp
   Query.cpp
   Solution.cpp
)
set(HEADERS
   Component.hpp
   Port.hpp
   Query.hpp
   Solution.hpp
)

set(Gecode_LIBRARIES
   gecodegist
   gecodedriver
   gecodeminimodel
   gecodesearch
   gecodeint
   gecodekernel
   gecodesupport
)

rock_library(${PROJECT_NAME} 
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPS_PKGCONFIG base-lib
    LIBS ${Gecode_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} ${Gecode_LIBRARIES})

rock_executable(gecode_composition_management-test
        SOURCES Solution.cpp
        DEPS ${PROJECT_NAME}
)
