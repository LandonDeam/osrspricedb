cmake_minimum_required(VERSION 3.23)
project(osrs_price_db CXX)

find_package(Poco REQUIRED)
find_package(mysql-concpp REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp src/main.h
        src/get_api.cpp src/get_api.h
        src/item.cpp src/item.h
        src/item_map.cpp src/item_map.h
        src/price_update.cpp src/price_update.h
        src/price_series.cpp src/price_series.h
        src/mysql.cpp src/mysql.h
        src/env.cpp src/env.h
        src/utils.cpp src/utils.h)
target_link_libraries(${PROJECT_NAME} Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::Crypto Poco::NetSSL mysql::concpp)

install(TARGETS osrs_price_db DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
