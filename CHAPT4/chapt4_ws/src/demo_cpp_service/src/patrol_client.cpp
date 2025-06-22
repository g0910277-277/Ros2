#include "rclcpp/rclcpp.hpp"
#include "chapt4_interfaces/srv/patrol.hpp"
#include <chrono>
#include <ctime>
#include "rcl_interfaces/msg/parameter.hpp"
#include "rcl_interfaces/msg/parameter_value.hpp"
#include "rcl_interfaces/msg/parameter_type.hpp"
#include "rcl_interfaces/srv/set_parameters.hpp"

using namespace std::chrono_literals;
using Patrol = chapt4_interfaces::srv::Patrol;
using SetParameters = rcl_interfaces::srv::SetParameters;

class PatrolClient: public rclcpp::Node{
public:
    explicit PatrolClient(const std::string node_name):Node(node_name){
        srand(time(nullptr));
        patrol_client = this->create_client<Patrol>("patrol");
        timer = this->create_wall_timer(10s, [&]()->void{
            while(!this->patrol_client->wait_for_service(1s)){
                if(!rclcpp::ok()){
                    RCLCPP_ERROR(this->get_logger(), "rclcpp wrong!");
                    return;
                }
                RCLCPP_ERROR(this->get_logger(), "wait for service!");
            }
            auto request = std::make_shared<Patrol::Request>();
            request->target_x = rand() % 15;
            request->target_y = rand() % 15;

            patrol_client->async_send_request(request, [&](rclcpp::Client<Patrol>::SharedFuture result_future)->void{
                auto response = result_future.get();
                if(response->result == Patrol::Response::SUCCESS){
                    // send success
                }
                else{
                    // send failed
                }
            }); 
        });
    }

    SetParameters::Response::SharedPtr call_set_parameters(rcl_interfaces::msg::Parameter& param){
        auto param_client = this->create_client<SetParameters>("/turtle_controller/set_parameters");
        while(!param_client->wait_for_service(1s)){
            if(!rclcpp::ok()){
                RCLCPP_ERROR(this->get_logger(), "rclcpp wrong!");
                return nullptr;
            }
            RCLCPP_ERROR(this->get_logger(), "wait for service!");
        }

        auto request = std::make_shared<SetParameters::Request>();
        request->parameters.push_back(param);

        //sync send
        auto future = param_client->async_send_request(request);
        rclcpp::spin_until_future_complete(this->get_node_base_interface(), future);
        auto response = future.get();
        return response;
    }

    void update_server_param_k(double k){
        auto param = rcl_interfaces::msg::Parameter();
        param.name = "k";

        auto param_value = rcl_interfaces::msg::ParameterValue();
        param_value.type = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;
        param_value.double_value = k;
        param.value = param_value;

        auto response = this->call_set_parameters(param);
        if(response == nullptr){
            RCLCPP_INFO(this->get_logger(), "parameter update failed!");
            return;
        }
        for(auto result: response->results){
            if(result.successful == false){
                RCLCPP_INFO(this->get_logger(), "parameter update failed, %s!", result.reason.c_str());
            }
            else{
                RCLCPP_INFO(this->get_logger(), "parameter update success!");
            }
        }
    }

private:
    rclcpp::TimerBase::SharedPtr timer;
    rclcpp::Client<Patrol>::SharedPtr patrol_client;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PatrolClient>("turtle_circle");
    node->update_server_param_k(4.0);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}