
#rock_testsuite(test_CSP_OWL main_owl.cpp
#    DEPS ${PROJECT_NAME} )

rock_testsuite(testCSP main.cpp tests.hpp dump.hpp state_machines.hpp constraints.hpp
    DEPS ${PROJECT_NAME} )


rock_testsuite(testInstance main_instance.cpp
    DEPS ${PROJECT_NAME} )

