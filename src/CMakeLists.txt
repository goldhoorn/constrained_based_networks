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
   Logger.cpp
   EventModelHandler.cpp
    XML.cpp
   #  Import.cpp
)

set(HEADERS
    XML.hpp
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
   SpecializedComponent.hpp
   Logger.hpp
   EventModelHandler.hpp
   #   Import.hpp
)

find_library(gecode1 NAMES gecodegist      PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode2 NAMES gecodedriver    PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode3 NAMES gecodeminimodel PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode4 NAMES gecodesearch    PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode5 NAMES gecodeint       PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode6 NAMES gecodekernel    PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode7 NAMES gecodesupport   PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode8 NAMES gecodeflatzinc  PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode9 NAMES gecodefloat     PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )
find_library(gecode10 NAMES gecodeset      PATHS ${CMAKE_INSTALL_TARGET}/lib REQUIRED )

set(Gecode_LIBRARIES
    ${gecode1} ${gecode2} ${gecode3} ${gecode4} ${gecode5} ${gecode6} ${gecode7} ${gecode8} ${gecode9} ${gecode10}
)


rock_library(${PROJECT_NAME} 
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPS_PKGCONFIG base-lib owlapi graph_analysis libxml++-2.6 
    LIBS ${Gecode_LIBRARIES} ${Boost_LIBRARIES}
)


target_link_libraries(${PROJECT_NAME} ${Gecode_LIBRARIES})

rock_executable(cbn_gexf_to_dot cbn_gexf_to_dot.cpp DEPS ${PROJECT_NAME} )

rock_executable(cbn_class_on_model SOURCES cbn_class_on_model.cpp DEPS ${PROJECT_NAME})

rock_executable(cbn_instance_on_model cbn_instance_on_model.cpp DEPS ${PROJECT_NAME})

#rock_executable(${PROJECT_NAME}-test
#        SOURCES ClassSolution.cpp
#        DEPS ${PROJECT_NAME}
#)
