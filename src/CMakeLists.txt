set(SOURCES
   solution.cpp
)
set(HEADERS
   component.h
   port.h
   query.h
)

add_executable(gecode-composition_management ${SOURCES} ${HEADERS})
target_link_libraries(gecode-composition_management gecodegist gecodedriver gecodeminimodel gecodesearch gecodeint gecodekernel gecodesupport)