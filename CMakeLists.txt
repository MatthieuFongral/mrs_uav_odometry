cmake_minimum_required(VERSION 2.8.3)
project(mrs_uav_odometry)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  geometry_msgs
  tf2_geometry_msgs
  std_msgs
  nav_msgs
  mrs_lib
  cmake_modules
  mrs_msgs
  nodelet
  dynamic_reconfigure
  )

find_package(mavros_msgs REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

# please, NEVER commit those alternative flags with specific overrides of optimization
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O0 -g")

# include Eigen3
find_package(Eigen3 REQUIRED)
set(Eigen_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIRS})
set(Eigen_LIBRARIES ${Eigen_LIBRARIES})

###################################
## catkin specific configuration ##
###################################

generate_dynamic_reconfigure_options(
  cfg/odometry_dynparam.cfg
  )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES Odometry
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs tf2_geometry_msgs geometry_msgs mrs_lib mrs_msgs
  DEPENDS mavros_msgs Eigen
  )

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${mavros_msgs_INCLUDE_DIRS}
  ${Eigen_INCLUDE_DIRS}
  )

## Declare a C++ executable
add_library(Odometry src/support.cpp src/StateEstimator.cpp src/AltitudeEstimator.cpp src/HeadingEstimator.cpp src/odometry.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(Odometry
  ${catkin_LIBRARIES}
  ${mavros_msgs_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(Odometry ${PROJECT_NAME}_gencfg)

## Declare a C++ executable
add_library(RtkRepublisher src/rtk_republisher.cpp src/support.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(RtkRepublisher
  ${catkin_LIBRARIES}
  ${mavros_msgs_LIBRARIES}
  )

## Declare a C++ executable
add_library(TFConnectorDummy src/tf_connector_dummy.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(TFConnectorDummy
  ${catkin_LIBRARIES}
  ${mavros_msgs_LIBRARIES}
  )
