cmake_minimum_required(VERSION 3.0.2)
project(the_receptionist)

add_compile_options(-std=c++17)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  dialogflow_ros_msgs
  roslint
  std_srvs
  std_msgs
  sound_play
  actionlib
  actionlib_msgs
  geometry_msgs 
  message_generation 
  cv_bridge 
  image_transport
  tf
  behaviortree_cpp_v3
)

add_message_files(
  FILES
  Info.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES the_receptionist behaviortree_cpp_v3
	CATKIN_DEPENDS roscpp dialogflow_ros_msgs actionlib_msgs message_runtime
	DEPENDS
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${ZMQ_INCLUDE_DIRS}
)

link_directories(
	${PCL_LIBRARY_DIRS}
)

add_definitions(${PCL_DEFINITIONS})

add_library(the_receptionist
  src/the_receptionist/DialogInterfaceTR.cpp
)

list(APPEND plugin_libs
  
)

add_library(NavegarTR SHARED src/behavior_tree_nodes/NavegarTR.cpp)
add_library(Navegar2TR SHARED src/behavior_tree_nodes/Navegar2TR.cpp)
add_library(Navegar3TR SHARED src/behavior_tree_nodes/Navegar3TR.cpp)
add_library(ObservarTR SHARED src/behavior_tree_nodes/ObservarTR.cpp)
add_library(DataCheckTR SHARED src/behavior_tree_nodes/DataCheckTR.cpp)
add_library(DataDumpTR SHARED src/behavior_tree_nodes/DataDumpTR.cpp)
add_library(ControlDumpTR SHARED src/behavior_tree_nodes/ControlDumpTR.cpp)
add_library(ControlDataTR SHARED src/behavior_tree_nodes/ControlDataTR.cpp)
add_library(ControlObservadorTR SHARED src/behavior_tree_nodes/ControlObservadorTR.cpp)



add_dependencies(the_receptionist ${catkin_EXPORTED_TARGETS} sound_play_generate_messages_cpp)
target_link_libraries(the_receptionist ${catkin_LIBRARIES})

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/robocup-home-education-los-ultramarinos.cpp
# )

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide

add_executable(ObservadorPersonasTR src/the_receptionist/ObservadorPersonasTR.cpp)
add_executable(angleCalcTR src/the_receptionist/angleCalcTR.cpp)
add_executable(data_collectorTR src/the_receptionist/data_collectorTR.cpp)
add_executable(TextToSpeechTR src/the_receptionist/TextToSpeechTR.cpp)
add_executable(The_Receptionist src/The_Receptionist.cpp)

target_link_libraries( ObservadorPersonasTR
${catkin_LIBRARIES}
)

target_link_libraries( angleCalcTR
${catkin_LIBRARIES}
)

target_link_libraries( data_collectorTR
${catkin_LIBRARIES}
)

target_link_libraries( TextToSpeechTR
${catkin_LIBRARIES}
${PROJECT_NAME}
)

target_link_libraries( The_Receptionist
${catkin_LIBRARIES}
)






## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )


##add_executable(Monologo_node src/the_receptionist.cpp)


##add_dependencies(Monologo_node ${catkin_EXPORTED_TARGETS} )


##target_link_libraries(Monologo_node ${catkin_LIBRARIES} the_receptionist ${ZMQ_LIBRARIES})


list(APPEND plugin_libs
  NavegarTR
  Navegar2TR
  Navegar3TR
  ObservarTR
  DataCheckTR
  DataDumpTR
  ControlDataTR
  ControlDumpTR
  ControlObservadorTR
  angleCalcTR
)

foreach(bt_plugin ${plugin_libs})
  target_link_libraries(${bt_plugin} ${catkin_LIBRARIES} the_receptionist)
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()


