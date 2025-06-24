# 6.2

### install urdf vscode plugins
ros2 pkg create fishbot_description --license Apache-2.0

sudo apt install liburdfdom-tools
sudo apt install graphviz

urdf_to_graphiz first_robot.urdf


### run rviz && install
sudo apt install ros-$ROS_DISTRO-joint-state-publisher
sudo apt install ros-$ROS_DISTRO-robot-state-publisher


use launch start 3 node:
joint-state-publisher
robot-state-publisher


### Xacro simplify URDF
sudo apt install ros-$ROS_DISTRO-xacro
xacro first_robot.xacro



ros2 launch fishbot_description display_robot_launch.py model:=/home/gaojun/learning/Ros2/CHAPT6/chapt6_ws/install/fishbot_description/share/fishbot_description/urdf/fishbot/fishrot.urdf.xacro 