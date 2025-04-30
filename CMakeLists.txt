cmake_minimum_required(VERSION 3.23)
project(osrs_price_db CXX)
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG RELEASE)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON)

find_package(Poco REQUIRED)
find_package(mysql-concpp REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

add_executable(${PROJECT_NAME} ${HEADERS} ${IMPLEMENTATIONS} ${SQL})
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL mysql::concpp rapidjson)

# install(TARGETS osrs_price_db DESTINATION ".")
