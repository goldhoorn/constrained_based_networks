
#rock_testsuite(test_CSP_OWL main_owl.cpp
#    DEPS ${PROJECT_NAME} )

rock_testsuite(testCSP main.cpp tests.hpp dump.hpp state_machines.hpp constraints.hpp
    DEPS ${PROJECT_NAME}  )

target_link_libraries(testCSP ${Boost_LIBRARIES})

#rock_testsuite(Specialized_CSP specialized.cpp tests.hpp dump.hpp state_machines.hpp constraints.hpp
#    DEPS ${PROJECT_NAME} )



#rock_testsuite(testInstance main_instance.cpp
#    DEPS ${PROJECT_NAME} )

