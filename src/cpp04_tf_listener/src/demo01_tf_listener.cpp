/*
    需求：先发布laser到base_link,再发布camera到base_link,
    求解laser到camera的tf变换关系。
    流程：
        1.包含头文件；
        2.初始化ROS2客户端；
        3.自定义节点类；
          3-1.创建一个缓存对象，融合多个坐标系相对关系为一棵坐标树；
          3-2.创建一个监听器，绑定缓存对象，会将所有的广播器广播的数据写入缓存；
          3-3.编写一个定时器，循环实现转换。                
        4.调用spain函数，并传入节点对象；
        5.资源释放。 


*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals; // 使用命名空间，使用时间单位为ms
// /opt/ros/humble/include/rclcpp/rclcpp.hpp

// 3.定义节点类；
class TFListener : public rclcpp::Node
{
public:
  TFListener(): Node("TFListener_node_cpp")
  {
    // 3-1.创建一个缓存对象，融合多个坐标系相对关系为一棵坐标树；
    buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    
    // 3-2.创建一个监听器，绑定缓存对象，会将所有的广播器广播的数据写入缓存；
    listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_,this);

    // 3-3.编写一个定时器，循环实现转换。
    timer_ = this->create_wall_timer(1s,std::bind(&TFListener::on_timer, this));
  }
private:
    void on_timer(){
      //实现坐标转换
      try{
        //buffer_->lookupTransform

        //const std::string & target_frame, 
        // const std::string & source_frame,
        // const rclcpp::Time & time,
        /* code */
        auto ts = buffer_->lookupTransform("camera", "laser", tf2::TimePointZero);
        RCLCPP_INFO(this->get_logger(),"转换完成的坐标帧消息");
        RCLCPP_INFO(this->get_logger(),
        "新坐标帧：父坐标系：%s,子坐标系：%s,偏移量（%.2f, %.2f, %.2f）",
        ts.header.frame_id.c_str(),//camera
        ts.child_frame_id.c_str(),//laser
        ts.transform.translation.x,
        ts.transform.translation.y,
        ts.transform.translation.z);
      }
      catch(const tf2::LookupException & e)
      {
        //当
        RCLCPP_INFO(this->get_logger(),"坐标转换失败，失败原因：%s",e.what());
      }

      
    }
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  // 2.初始化 ROS2 客户端；
  rclcpp::init(argc, argv);
  // 4.调用spin函数，并传入节点对象指针。
  rclcpp::spin(std::make_shared<TFListener>());
  // 5.释放资源；
  rclcpp::shutdown();
  return 0;
}