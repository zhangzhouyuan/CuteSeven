cmake_minimum_required(VERSION 2.8.3)
project(tx_iot)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES xf_voice
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)
#include_directories(include ${catkin_INCLUDE_DIRS} /Robot/Tencent_iot_SDK/include)
#link_directories(/Robot/Tencent_iot_SDK/lib)

#add_executable(tx_iot src/tx_iot.c)
#target_link_libraries(tx_iot ${catkin_LIBRARIES} -ltxdevicesdk -lpthread -ldl -lstdc++)
#add_dependencies(tx_iot tx_iot_generate_messages_cpp)

#add_executable(wakeup src/wakeup.cpp)
#target_link_libraries(wakeup ${catkin_LIBRARIES}   -lmraa)
#add_dependencies(wakeup xf_voice_generate_messages_cpp)
