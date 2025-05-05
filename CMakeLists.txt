cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0167 NEW)
project(JetService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Boost 1.87.0 CONFIG REQUIRED COMPONENTS system filesystem json)
find_package(SQLite3 CONFIG REQUIRED)

include_directories(
    include 
    thirdparty 
    ${Boost_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      Boost::system
      Boost::filesystem
      Boost::json
      SQLite::SQLite3
)
