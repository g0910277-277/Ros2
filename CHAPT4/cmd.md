ros2 pkg create chapt4_interfaces --dependencies sensor_msgs rosidl_default_generators --license Apache-2.0

# picture
https://github.com/ultralytics/yolov5/raw/master/data/images/zidane.jpg

ros2 pkg create demo_python_service --build-type ament_python --dependencies rclpy chapt4_interfaces --license Apache-2.0

# setup.py add copy picture 
('share/' + package_name+"/resource", ['resource/default.jpg']),



# 4.3
ros2 pkg create demo_cpp_service --dependencies chapt4_interfaces rclcpp geometry_msgs turtlesim --license Apache-2.0

# 4.4 param set


# 4.6 launch
ros2 launch demo_cpp_service demo_launch.py launch_arg_bg:=255
