#find_package(Boost 1.48 COMPONENTS serialization system REQUIRED)

set(SOURCES
   Pool.cpp
   Composition.cpp
   Task.cpp
   Component.cpp
   DataService.cpp
   ClassSolution.cpp
   PortHandler.cpp
   StateMachine.cpp
   InstanceSolution.cpp
)

set(HEADERS
   Pool.hpp
   Composition.hpp
   Task.hpp
   DataService.hpp
   Component.hpp
   Port.hpp
   ClassSolution.hpp
   PortHandler.hpp
   StateMachine.hpp
   InstanceSolution.hpp
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
    LIBS ${Gecode_LIBRARIES} ${Boost_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} ${Gecode_LIBRARIES})

#rock_executable(${PROJECT_NAME}-test
#        SOURCES ClassSolution.cpp
#        DEPS ${PROJECT_NAME}
#)
