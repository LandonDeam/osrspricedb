cmake_minimum_required(VERSION 3.23)
project(osrs_price_db CXX)

find_package(Poco REQUIRED)
find_package(mysql-concpp REQUIRED)
# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

file(GLOB IMPLEMENTATIONS src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB SQL database/*.sql)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_BINARY_DIR "../../build")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_BINARY_DIR "../build")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

add_executable(${PROJECT_NAME} ${HEADERS} ${IMPLEMENTATIONS} ${SQL})
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL mysql::concpp)

# install(TARGETS osrs_price_db DESTINATION ".")
