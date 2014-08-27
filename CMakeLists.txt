cmake_minimum_required(VERSION 2.8.3)
project(ros_viz_3d)

set(CMAKE_CXX_FLAGS "-std=c++0x")

find_package(catkin REQUIRED COMPONENTS rviz roscpp)

find_package(Qt4 COMPONENTS QtCore REQUIRED)
include(${QT_USE_FILE})

include_directories(${catkin_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ros_viz_3d
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

add_executable(ros_viz_3d ros_viz_3d.cpp main.cpp)

target_link_libraries(ros_viz_3d ${catkin_LIBRARIES})
