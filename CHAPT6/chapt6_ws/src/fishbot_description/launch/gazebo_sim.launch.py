import launch
import launch.launch_description_sources
import launch_ros
from ament_index_python.packages import get_package_share_directory
import os

import launch_ros.parameter_descriptions

def generate_launch_description():
    # 获得功能包的share路径
    urdf_package_path = get_package_share_directory('fishbot_description')
    
    # 默认路径
    default_xacro_path = os.path.join(urdf_package_path, 'urdf', 'fishbot', 'fishrot.urdf.xacro')
    # default_rviz_config_path = os.path.join(urdf_package_path, 'config', 'display_robot_model.rviz')
    default_gazebo_world_path = os.path.join(urdf_package_path, 'world', 'room_world.world')
    
    # 声明model参数，可在外部传入
    action_declare_arg_mode_xacro_path = launch.actions.DeclareLaunchArgument(
        name = 'model', default_value=str(default_xacro_path), description='param'
    )
    
    # xacro -> urdf
    substitution_command_result_xacro = launch.substitutions.Command(['xacro ', launch.substitutions.LaunchConfiguration('model')])
    robot_description_value = launch_ros.parameter_descriptions.ParameterValue(
        substitution_command_result_xacro, value_type=str
    )


    action_robot_state_publisher = launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':robot_description_value}]
    )
    
    action_launch_gazebo = launch.actions.IncludeLaunchDescription(
        launch.launch_description_sources.PythonLaunchDescriptionSource(
            [get_package_share_directory('gazebo_ros'), '/launch/', 'gazebo.launch.py']
        ),
        launch_arguments=[('world', default_gazebo_world_path), ('verbose', 'true')]
    )
    
    return launch.LaunchDescription([
        action_declare_arg_mode_xacro_path,
        action_robot_state_publisher,
        action_launch_gazebo,
    ])