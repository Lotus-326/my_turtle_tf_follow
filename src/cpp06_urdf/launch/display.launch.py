from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
# 文件包含相关-------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关----------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关----------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# 获取功能包下share目录路径-------
from ament_index_python.packages import get_package_share_directory
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    """
        需求：加载urdf文件并在rviz2中显示机器人模型
        核心：
            1.启动robot_state_publisher节点，该节点要以参数的方式加载urdf文件内容；
            2.启动rviz2节点
        优化：
            1.添加 joint_state_publisher 节点 （当机器人有非固定关节时，必须包含该节点） ;
            2.设置 rviz2 的默认配置文件；
            3.动态的传入urdf文件，把urdf文件封装为参数。
    """

    # 1.启动robot_state_publisher节点，该节点要以参数的方式加载urdf文件内容；
    # p_value = ParameterValue(Command(["xacro ",get_package_share_directory("cpp06_urdf") + "/urdf/urdf/demo01_helloworld.urdf"]))
    #优化3    
    model = DeclareLaunchArgument(name="model",default_value=get_package_share_directory("cpp06_urdf") + "/urdf/urdf/demo01_helloworld.urdf")
    p_value = ParameterValue(Command(["xacro ",LaunchConfiguration("model",)]))

    robot_state_pub = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{"robot_description": p_value}]
    )
    # 优化1
    joint_state_pub = Node(
        package ="joint_state_publisher",
        executable ="joint_state_publisher",

    )
    # 2.启动rviz2节点 优化2
    rviz2 = Node(package="rviz2",
        executable="rviz2",
        argument = ["-d",get_package_share_directory("cpp06_urdf") + "/rviz/urdf.rviz"]
    )
    return LaunchDescription([model,robot_state_pub,rviz2,joint_state_pub])