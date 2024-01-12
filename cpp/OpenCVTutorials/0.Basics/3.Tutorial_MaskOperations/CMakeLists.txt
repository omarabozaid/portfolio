cmake_minimum_required(VERSION 3.10)
project(TutorialMasking)

# Manually find OpenCV components
find_package(OpenCV REQUIRED core highgui)

# Print the paths
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")
message("OpenCV_LIBRARY_DIRS: ${OpenCV_LIBRARY_DIRS}")
message("OpenCV_DEFINITIONS: ${OpenCV_DEFINITIONS}")

add_executable(TutorialMasking main.cpp)
target_link_libraries(TutorialMasking PRIVATE ${OpenCV_LIBS})
