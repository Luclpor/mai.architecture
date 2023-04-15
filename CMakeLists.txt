cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (AUTH_SERVICE "auth_service")
SET(SERVICE_SERVICE "service_service")
SET(ORDER_SERVICE "order_service")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(${AUTH_SERVICE} auth_main.cpp
                                config/config.cpp
                                database/database.cpp
                                database/auth.cpp)


target_include_directories(${AUTH_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${AUTH_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${AUTH_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${AUTH_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                             
target_link_libraries(${AUTH_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)

add_executable(${SERVICE_SERVICE} service_main.cpp
                             config/config.cpp
                             database/database.cpp
                             database/service.cpp)


target_include_directories(${SERVICE_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${SERVICE_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${SERVICE_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${SERVICE_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                          
target_link_libraries(${SERVICE_SERVICE} PRIVATE 
                          ${CMAKE_THREAD_LIBS_INIT} 
                          ${Poco_LIBRARIES}
                          "PocoData"
                          "PocoDataMySQL"
                          "mysqlclient"
                          ZLIB::ZLIB)

add_executable(${ORDER_SERVICE} order_main.cpp
                          config/config.cpp
                          database/database.cpp
                          database/order.cpp)


target_include_directories(${ORDER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${ORDER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${ORDER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ORDER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
                       
target_link_libraries(${ORDER_SERVICE} PRIVATE 
                       ${CMAKE_THREAD_LIBS_INIT} 
                       ${Poco_LIBRARIES}
                       "PocoData"
                       "PocoDataMySQL"
                       "mysqlclient"
                       ZLIB::ZLIB)




