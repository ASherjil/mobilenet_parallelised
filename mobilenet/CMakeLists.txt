cmake_minimum_required(VERSION 3.10)
project(mobilenet)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all source files in the current directory
file(GLOB SOURCES "*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# If OpenMP is found, link it to the target
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()


