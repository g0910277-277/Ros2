#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h" //rf2 quaternion class
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/utils.h"
#include <chrono>
using namespace std::chrono_literals;

class TFListener: public rclcpp::Node{
public:
    TFListener(): Node("tf_broadcast"){
        buffer = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        listener = std::make_shared<tf2_ros::TransformListener>(*buffer, this);
        timer = this->create_wall_timer(1s, std::bind(&TFListener::getTransform, this));
    }

    void getTransform(){
        try{
            const auto transform = buffer->lookupTransform("base_link", "target_point", this->get_clock()->now(), rclcpp::Duration::from_seconds(1));
            
            auto translation = transform.transform.translation;
            auto rotation = transform.transform.rotation;
            double y, p, r;
            tf2::getEulerYPR(rotation, y, p, r);
            RCLCPP_INFO(get_logger(), "translation: %f, %f %f", translation.x, translation.y, translation.z);
            RCLCPP_INFO(get_logger(), "rotation: %f, %f %f", y, p, r);

        }
        catch(const std::exception& e){
            RCLCPP_WARN(get_logger(), "%s", e.what());
        }        
    }


private:
    std::shared_ptr<tf2_ros::TransformListener> listener;

    rclcpp::TimerBase::SharedPtr timer;

    std::shared_ptr<tf2_ros::Buffer> buffer;
};

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TFListener>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


