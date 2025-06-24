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


### 6.2.5
ros2 launch fishbot_description display_robot_launch.py model:=/home/gaojun/learning/Ros2/CHAPT6/chapt6_ws/install/fishbot_description/share/fishbot_description/urdf/fishbot/fishrot.urdf.xacro 


### 6.2.6
用虚拟部件链接机器人，一个不存在的东西设定好高度从而将轮子贴地

# 6.3
### 6.3.1 添加碰撞属性
碰撞属性和isual可以用相同配置，碰撞边缘和isual边缘是一样的

### 6.3.2 添加质量和惯性(视频里有惯性和旋转惯性公式，以后在详细了解)
common_inertial.xacro

在base.urdf.xacro里
<xacro:include filename="$(find fishbot_description)/urdf/fishbot/common_inertial.xacro"/>
因为是base用

rviz不能显示Mass和Inertial，因为用的是foxy，据说是humble才有的，所以其他的惯性质量不加了


# 6.4 Gazebo完成机器人仿真

### 6.4.1 安装使用gazebo构建世界
sudo apt install gazebo (fish的humble)

视频中下载和下载模型方式因为DISTRO差异不太一样，我的是ubuntu20.04 foxy

安装 Gazebo 11：
sudo apt install gazebo11 libgazebo11-dev

安装 ROS 2 Foxy 插件包
sudo apt install ros-foxy-gazebo-ros-pkgs
sudo apt install ros-foxy-gazebo-ros-pkgs

Gazebo 需额外模型库显示物体
mkdir -p ~/.gazebo

#### clone到~/.gazebo/gazebo_models
git clone https://github.com/osrf/gazebo_models ~/.gazebo/gazebo_models  # 下载官方模型:cite[4]:cite[5]:cite[6]

或者使用gitee加速
git clone https://gitee.com/ohhuo/gazebo_models.git ~/.gazebo/models

防止.git被误识别为模型
rm -rf ~/.gazebo/models/.git


### 6.4.2 在gazebo中加载机器人模型
机器人使用urdf，gazebo使用sdf，因此需要先做转换
sudo apt install ros-$ROS_DISTRO-gazebo-ros-pkgs

xacro->urdf->sdf

### 6.4.3 使用Gazebo标签扩展URDF

### 6.4.4 使用两轮差速插件控制机器人

### 6.4.5 激光雷达传感器仿真

### 6.4.6 惯性测量传感器仿真

### 6.4.7 深度相机传感器仿真

# 6.5 ros2_control

### 6.5.1 ros2_control安装介绍

### 6.5.2 使用gazebo接入ros2_control

### 6.5.3 使用关节状态发布控制器

### 6.5.4 使用力控制器控制论子

### 6.5.5 使用两轮差速控制器控制机器人