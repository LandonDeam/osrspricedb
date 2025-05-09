cmake_minimum_required(VERSION 3.23)
project(osrs_price_db CXX)
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG RELEASE)

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


find_package(Poco REQUIRED)
find_package(mysql-concpp REQUIRED)
find_package(pugixml REQUIRED)
find_package(tidy-html5 REQUIRED)

file(GLOB IMPLEMENTATIONS src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB SQL database/*.sql)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

add_executable(${PROJECT_NAME} ${HEADERS} ${IMPLEMENTATIONS} ${SQL})
target_link_libraries(${PROJECT_NAME} PUBLIC Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL mysql::concpp pugixml::pugixml tidy-html5::tidy-html5)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/generators")

# install(TARGETS osrs_price_db DESTINATION ".")
