/*
    需求：读取bag文件数据，并将数据输出在终端
    流程：
        1.包含头文件；
        2.初始化ROS2客户端；
        3.自定义节点类；
          3-1.创建回放对象
          3-2.设置被读取的文件
          3-3.读消息
          3-4.关闭文件                            
        4.调用spain函数，并传入节点对象；
        5.资源释放。 


*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/reader.hpp"
#include "geometry_msgs/msg/twist.hpp"
// /opt/ros/humble/include/rclcpp/rclcpp.hpp

// 3.定义节点类；
class SimpleBagPlayer : public rclcpp::Node
{
public:
    SimpleBagPlayer(): Node("simple_bag_play_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "节点消息回放创建成功");
        // 3-1.创建回放对象
        reader_ = std::make_unique<rosbag2_cpp::Reader>();
        // 3-2.设置被读取的文件
        reader_->open("my_bag");
        // 3-3.读消息
        while(reader_->has_next()){
            auto twist = reader_->read_next<geometry_msgs::msg::Twist>();
            RCLCPP_INFO(this->get_logger(), "线速度：%.2f,角速度：%.2f", twist.linear.x, twist.angular.z);   
        }
        // 3-4.关闭文件
        reader_->close();
    }
private:
    std::unique_ptr<rosbag2_cpp::Reader> reader_;
};

int main(int argc, char * argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc, argv);
    // 4.调用spin函数，并传入节点对象指针。
    rclcpp::spin(std::make_shared<SimpleBagPlayer>());
    // 5.释放资源；
    rclcpp::shutdown();
    return 0;
}