cmake_minimum_required(VERSION 3.15)
project(osrs_price_db CXX)


find_package(Poco REQUIRED)

FILE(GLOB CPP_SOURCES src/*.cpp)
FILE(GLOB CPP_HEADERS src/*.h)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${CPP_HEADERS})
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL)

install(TARGETS osrs_price_db DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
