import launch
import launch_ros
from ament_index_python.packages import get_package_share_directory
import os

import launch_ros.parameter_descriptions

def generate_launch_description():
    
    urdf_package_path = get_package_share_directory('fishbot_description')
    default_urdf_path = os.path.join(urdf_package_path, 'urdf', 'first_robot.urdf')
    default_xacro_path = os.path.join(urdf_package_path, 'urdf', 'first_robot.xacro')
    default_rviz_config_path = os.path.join(urdf_package_path, 'config', 'display_robot_model.rviz')
    
    # declare parameter
    action_declare_arg_mode_path = launch.actions.DeclareLaunchArgument(
        name = 'model', default_value=str(default_urdf_path), description='param'
    )
    
    action_declare_arg_mode_xacro_path = launch.actions.DeclareLaunchArgument(
        name = 'model_xacro', default_value=str(default_xacro_path), description='param'
    )
    

    substitution_command_result = launch.substitutions.Command(['cat ', launch.substitutions.LaunchConfiguration('model')])    
    robot_description_value = launch_ros.parameter_descriptions.ParameterValue(
        substitution_command_result, value_type=str
    )
    """
    xacro has some problem
    substitution_command_result_xacro = launch.substitutions.Command(['xacro ', launch.substitutions.LaunchConfiguration('model_xacro')])
    robot_description_value = launch_ros.parameter_descriptions.ParameterValue(
        substitution_command_result_xacro, value_type=str
    )
    """
    action_robot_state_publisher = launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':robot_description_value}]
    )

    action_joint_state_publisher = launch_ros.actions.Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
    )
    
    action_rviz_node = launch_ros.actions.Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', default_rviz_config_path],
    )
    
    return launch.LaunchDescription([
        action_declare_arg_mode_path,
        action_declare_arg_mode_xacro_path,
        action_robot_state_publisher,
        action_joint_state_publisher,
        action_rviz_node,
    ])