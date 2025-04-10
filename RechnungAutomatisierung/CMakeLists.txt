cmake_minimum_required(VERSION 3.10)
project(RechnungAutomatisierung)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ODBC package
find_package(ODBC REQUIRED)

# Add source files
set(SOURCES
    main.cpp
    Application.cpp
    Database.cpp
    Utils.cpp
)

# Add header files
set(HEADERS
    Application.h
    Database.h
    Utils.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link ODBC libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ODBC::ODBC)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 