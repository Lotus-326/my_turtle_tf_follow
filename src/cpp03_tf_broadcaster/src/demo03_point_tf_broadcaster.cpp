/*
    需求：发布相对于laser坐标系的坐标点数据
    流程：
        1.包含头文件；
        2.初始化ROS2客户端；
        3.自定义节点类；
           3-1.创建发布方
           3-2.发布定时器
           3-3.回调函数中组织并发布消息             
        4.调用spain函数，并传入节点对象；
        5.资源释放。 


*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;
// /opt/ros/humble/include/rclcpp/rclcpp.hpp

// 3.定义节点类；
class PointBroadcaster : public rclcpp::Node
{
public:
    PointBroadcaster(): Node("point_broadcaster_node_cpp"),x(0.0)
    {
        // 3-1.创建发布方
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", 10);
        // 3-2.发布定时器
        timer_ = this->create_wall_timer(
            1s,
            std::bind(&PointBroadcaster::on_timer, this)
        );       
    }
private:
        // 3-3.回调函数中组织并发布消息
    void on_timer()
    {
        // 组织消息
        geometry_msgs::msg::PointStamped point_msg;
        point_msg.header.stamp = this->now();
        point_msg.header.frame_id = "laser";
        x += 0.05;
        point_msg.point.x = x;
        point_msg.point.y = 0.0;
        point_msg.point.z = -0.1;

        // 发布消息
        point_pub_->publish(point_msg);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double_t x;
};

int main(int argc, char const * argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数，并传入节点对象指针。
    rclcpp::spin(std::make_shared<PointBroadcaster>());
    // 5.释放资源；
    rclcpp::shutdown();
    return 0;
}