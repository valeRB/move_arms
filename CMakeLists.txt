cmake_minimum_required(VERSION 2.8.3)
project(move_arms)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS moveit_core moveit_ros_planning moveit_ros_planning_interface pluginlib cmake_modules)
find_package(Boost REQUIRED system filesystem date_time thread)

## Generate added messages and services
# sgenerate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package(
 CATKIN_DEPENDS 
 moveit_core
 moveit_ros_planning_interface
 interactive_markers
)

find_package(Eigen REQUIRED)

## Build talker and listener
include_directories(SYSTEM ${Boost_INCLUDE_DIR} ${EIGEN_INCLUDE_DIRS})
include_directories(${catkin_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})

add_executable(move_arms_test src/move_arms_test.cpp)
target_link_libraries(move_arms_test ${catkin_LIBRARIES} ${Boost_LIBRARIES})
# install(TARGETS planning_scene_tutorial DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

# install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

