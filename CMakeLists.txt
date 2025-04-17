cmake_minimum_required(VERSION 3.23)
project(osrs_price_db CXX)

find_package(Poco REQUIRED)
find_package(mysql-concpp REQUIRED)

file(GLOB IMPLEMENTATIONS src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB SQL database/*.sql)

add_executable(${PROJECT_NAME} ${HEADERS} ${IMPLEMENTATIONS} ${SQL})
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL mysql::concpp)

install(TARGETS osrs_price_db DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
