#include <iostream>
#include "rclcpp/rclcpp.hpp"
int main(int argc, char** argv)
{
    //std::cout << argc << std::endl;
    //std::cout << argv[0] << std::endl;
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("cpp_node");
    RCLCPP_INFO(node->get_logger(), "C++!");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}