import launch
import launch.launch_description_sources
import launch_ros

from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # 6. condition
    action_declare_startup_rqt = launch.actions.DeclareLaunchArgument('startup_rqt', default_value="False")  
    
    # 5. substitution
    startup_rqt = launch.substitutions.LaunchConfiguration('startup_rqt', default="False")
    
    # 1. action1 launch other launch
    multisim_launch_path = [get_package_share_directory('turtlesim'), '/launch/', 'multisim.launch.py']
    action_include_launch = launch.actions.IncludeLaunchDescription(
        launch.launch_description_sources.PythonLaunchDescriptionSource(
            multisim_launch_path
        )
    )
    
    # 2. action2 print
    action_log_info = launch.actions.LogInfo(msg=str(multisim_launch_path))
    
    # 3. action3 exec process
    actions_topic_list = launch.actions.ExecuteProcess(
        # cmd=['ros2', 'topic', 'list']
        condition=launch.conditions.IfCondition(startup_rqt), 
        cmd=['rqt']
    )
    
    # 4. action4 
    action_group = launch.actions.GroupAction([
        launch.actions.TimerAction(period=2.0, actions=[action_include_launch]),
        launch.actions.TimerAction(period=4.0, actions=[actions_topic_list]),
    ])
    
    
    
    return launch.LaunchDescription([
        action_declare_startup_rqt,
        action_log_info,
        action_group,
    ])