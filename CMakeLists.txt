cmake_minimum_required(VERSION 3.15)
project(osrs_price_db CXX)


find_package(Poco REQUIRED)

include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
        src/main.cpp src/main.h
        src/get_api.cpp src/get_api.h
        src/item_map.cpp src/item_map.h
        src/item.cpp src/item.h
        src/price.cpp src/price.h)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

install(TARGETS osrs_price_db DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
