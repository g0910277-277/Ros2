#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;
class TurtleControlNode: public rclcpp::Node{
public:
    explicit TurtleControlNode(const std::string node_name):Node(node_name){
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        subscriber_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, \
            std::bind(&TurtleControlNode::on_pose_received, this, std::placeholders::_1));
        // timer_ = this->create_wall_timer(1000ms, std::bind(&TurtleControlNode::timer_callback, this));
    }

    void on_pose_received(const turtlesim::msg::Pose::SharedPtr pose){
        auto msg = geometry_msgs::msg::Twist();
        auto current_x = pose->x;
        auto current_y = pose->y;
        RCLCPP_INFO(get_logger(), "current: x = %f, y = %f", current_x, current_y);

        auto distance = std::sqrt((target_x_ - current_x) * (target_x_ - current_x) + (target_y_ - current_y) * (target_y_ - current_y));
        auto angle = std::atan2((target_y_ - current_y), (target_y_ - current_y)) - pose->theta;

        if(distance > 0.1){
            if(fabs(angle) > 0.2){
                msg.angular.z = fabs(angle);
            }
            else{
                msg.linear.x = k_ * distance;
            }
        }
        
        if(msg.linear.x > max_speed_){
            msg.linear.x = max_speed_;
        }
        publisher_->publish(msg);
    }
    /*//call back
    void timer_callback(){
         auto msg = geometry_msgs::msg::Twist();
         msg.linear.x = 1.0;
         msg.angular.z = 0.5;
         publisher_->publish(msg);
    }
    */

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber_;

    double target_x_{1.0};
    double target_y_{1.0};
    double k_{1.0};
    double max_speed_{3.0};  


    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleControlNode>("turtle_circle");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}