# 5.1 TF
```bash
# video give but have problem
ros2 run tf2_ros static_transform_publisher --x 0.1 --y 0.0 --z 0.2 --roll 0.0 --pitch 0.0 --yaw 0.0 --frame-id base_link --child-frame-id base_laser

# terminal1
ros2 run tf2_ros static_transform_publisher 0.1 0.0 0.2 0.0 0.0 0.0 base_link base_laser

# terminal2
ros2 run tf2_ros static_transform_publisher 0.3 0.0 0.0 0.0 0.0 0.0 base_laser wall_point

# terminal3
ros2 run tf2_ros tf2_echo base_link wall_point
```


### install tools
```bash
sudo apt install ros-$ROS_DISTRO-mrpt2
or 
sudo apt install ros-foxy-mrpt2
3d-rotation-converter
```

ros2 run tf2_tools view_frames


# 5.2
sudo apt install ros-$ROS_DISTRO-tf-transformations

sudo pip3 install transforms3d
sudo pip3 install transforms3d -i https://pypi.mirrors.ustc.edu.cn/simple/


ros2 pkg create demo_python_tf --build-type ament_python --dependencies rclpy geometry_msgs tf_ros tf_transformations --license Apache-2.0





# 5.3
ros2 pkg create demo_cpp_tf --dependencies rclcpp tf2_ros geometry_msgs tf2_geometry_msgs --license Apache-2.0

# 5.4 install extra && update ini
sudo apt install ros-$ROS_DISTRO-rqt-tf-tree -y

rm -rf ls ~/.config/ros.org/rqt_gui.ini

