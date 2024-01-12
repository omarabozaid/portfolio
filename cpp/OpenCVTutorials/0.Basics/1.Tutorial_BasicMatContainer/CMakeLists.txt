cmake_minimum_required(VERSION 3.10)
project(TutorialMat)

# Manually find OpenCV components
find_package(OpenCV REQUIRED core highgui)

# Print the paths
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")
message("OpenCV_LIBRARY_DIRS: ${OpenCV_LIBRARY_DIRS}")
message("OpenCV_DEFINITIONS: ${OpenCV_DEFINITIONS}")

add_executable(TutorialMat main.cpp)
target_link_libraries(TutorialMat PRIVATE ${OpenCV_LIBS})
