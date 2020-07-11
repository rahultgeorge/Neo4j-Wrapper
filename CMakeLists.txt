cmake_minimum_required(VERSION 3.4)
project(neo4j_wrapper VERSION 1.0.0 DESCRIPTION "Neo4j Wrapper C++ ")
include(GNUInstallDirs)
if (APPLE)
    include_directories("/usr/local/opt/neo4j-client/include")
    include_directories("/usr/local/opt/openssl/lib")
    include_directories("/usr/local/opt/openssl/include")
endif (APPLE)
set (CMAKE_CXX_STANDARD 11)

include_directories("include")
include_directories("src")

find_library(NEO4J_CLIENT neo4j-client)

add_library(neo4j_wrapper SHARED src/neo4j_wrapper.cpp include/neo4j_wrapper.hpp)
set_target_properties(neo4j_wrapper PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER include/neo4j_wrapper.hpp
        COMPILE_FLAGS "-fno-rtti")
#configure_file(neo4j_wrapper.pc.in neo4j_wrapper.pc @ONLY)
#target_include_directories(neo4j_wrapper PRIVATE .)
install(TARGETS neo4j_wrapper
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(FILES ${CMAKE_BINARY_DIR}/neo4j_wrapper.pc
#   DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Neo4j Client

target_link_libraries(neo4j_wrapper PUBLIC "${NEO4J_CLIENT}")

if(APPLE)
    set_target_properties(neo4j_wrapper PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)

