/*
    需求：启动 turtlesim_node 节点，编写程序，发布乌龟（turtlel）相对于窗体（world）的位姿
    流程：
        1.包含头文件；
        2.初始化ROS2客户端；
        3.自定义节点类；
            3-1.创建一个动态广播器
            3-2.创建一个乌龟位姿订阅方
            3-3.在回调函数中，获取乌龟位姿信息并生成相对关系然后发布出去；                
        4.调用spain函数，并传入节点对象；
        5.资源释放。 


*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "turtlesim/msg/pose.hpp"
#include "tf2/LinearMath/Quaternion.h"
// /opt/ros/humble/include/rclcpp/rclcpp.hpp

// 3.定义节点类；
class TFDynaBroadcaster : public rclcpp::Node
{
public:
    TFDynaBroadcaster(): Node("tf_dyna_broadcaster_node_cpp")
    {
        // 3-1.创建一个动态广播器
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        // 3-2.创建一个乌龟位姿订阅方
        pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
            "turtle1/pose", 10, 
            std::bind(&TFDynaBroadcaster::do_pose, this, std::placeholders::_1));
       
    }
private:
    // 3-3.在回调函数中，获取乌龟位姿信息并生成相对关系然后发布出去；
    void do_pose(const turtlesim::msg::Pose & pose)
    {
        //组织消息
        geometry_msgs::msg::TransformStamped ts;

        ts.header.stamp = this->get_clock()->now();
        ts.header.frame_id = "world";

        ts.child_frame_id = "turtle1";
        
        ts.transform.translation.x = pose.x;
        ts.transform.translation.y = pose.y;
        ts.transform.translation.z = 0.0;
        
        //将欧拉角转换为四元数
        //乌龟的欧拉角只有 yaw 上的取值，没有 roll 和 pitch
        tf2::Quaternion qtn;
        qtn.setRPY(0, 0, pose.theta);

        ts.transform.rotation.x = qtn.x();
        ts.transform.rotation.y = qtn.y();
        ts.transform.rotation.z = qtn.z();
        ts.transform.rotation.w = qtn.w();
        
        //发布
        broadcaster_->sendTransform(ts);

    }
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数，并传入节点对象指针。
    rclcpp::spin(std::make_shared<TFDynaBroadcaster>());
    // 5.释放资源；
    rclcpp::shutdown();
    return 0;
}