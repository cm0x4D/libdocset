cmake_minimum_required(VERSION 2.8)
project(libDocset)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(HEADERS src/Docset
            src/docset.h
            src/DocsetObject
            src/docsetobject.h
            src/DocsetGroup
            src/docsetgroup.h
            src/libdocset.h
            src/LibDocset)
set(PRIVATE_HEADERS src/docset_p.hpp
                    src/docsetobject_p.hpp)
set(SOURCES src/docset.cpp
            src/docsetobject.cpp
            src/docsetgroup.cpp)

add_library(Docset SHARED ${SOURCES}
                          ${HEADERS}
                          ${PRIVATE_HEADERS})
target_link_libraries(Docset sqlite3)
install(FILES ${HEADERS} DESTINATION include)
install(TARGETS Docset LIBRARY DESTINATION lib)

add_subdirectory(tests)
