#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include "turtlesim/msg/pose.hpp"
#include "chapt4_interfaces/srv/patrol.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
using Patrol = chapt4_interfaces::srv::Patrol;
using SetParametersResult = rcl_interfaces::msg::SetParametersResult;


using namespace std::chrono_literals;
class TurtleControlNode: public rclcpp::Node{
public:
    explicit TurtleControlNode(const std::string node_name):Node(node_name){
        // declare default param
        this->declare_parameter("k", 1.0);
        this->declare_parameter("max_speed", 1.0);
        this->get_parameter("k", k_);
        this->get_parameter("max_speed", max_speed_); 
        //this->set_parameter(rclcpp::Parameter("k", 2.0));
        parameter_callback_handle = this->add_on_set_parameters_callback([&](const std::vector<rclcpp::Parameter> parameters)->SetParametersResult{
            SetParametersResult result;
            result.successful = true;
            for(const auto& parameter : parameters){
                // update member value
                RCLCPP_INFO(this->get_logger(), "update param value %s = %f", parameter.get_name().c_str(), parameter.as_double());
                if(parameter.get_name() == "k"){
                    k_ = parameter.as_double();
                }
                if(parameter.get_name() == "max_speed"){
                    max_speed_ = parameter.as_double();
                }
            }
            return result;
        });

        patrol_service = this->create_service<Patrol>("patrol", [&](const Patrol::Request::SharedPtr request, Patrol::Response::SharedPtr response)->void{
            if((request->target_x > 0 && request->target_x < 12) && (request->target_y > 0 && request->target_y < 12)){
                this->target_x_ = request->target_x;
                this->target_y_ = request->target_y;
                response->result = Patrol::Response::SUCCESS;
            } 
            else{
                response->result = Patrol::Response::FAIL;
            }
            //call extra cb function

        });

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


private:
    OnSetParametersCallbackHandle::SharedPtr parameter_callback_handle;   
    rclcpp::Service<Patrol>::SharedPtr patrol_service;
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