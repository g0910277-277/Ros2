#include <QApplication>
#include <QLabel>
#include <QString>
#include <rclcpp/rclcpp.hpp>
#include <status_interfaces/msg/system_status.hpp>

using SystemStatus = status_interfaces::msg::SystemStatus;

class SysStatusDisplay : public rclcpp::Node{
public:
    SysStatusDisplay(std::string node_name): Node(node_name){
        label = std::make_shared<QLabel>();
        subscriber = this->create_subscription<SystemStatus>("sys_status", 10, [&](const SystemStatus::SharedPtr msg)->void{
            label->setText(get_qstr_from_msg(msg));
        });
        label->setText(get_qstr_from_msg(std::make_shared<SystemStatus>()));
        label->show();
    }

    QString get_qstr_from_msg(const SystemStatus::SharedPtr msg){
        std::stringstream show_str;
        show_str << "========system status display tool========" << std::endl;
        show_str << "host name:\t" << msg->host_name << "\t" << std::endl;
        show_str << "timestamp:\t" << msg->stamp.sec << "\ts" << std::endl;
        show_str << "cpu usage:\t" << msg->cpu_percent<< "\t%" << std::endl;
        show_str << "memory usage:\t" << msg->memory_percent << "\t%" << std::endl;
        show_str << "memory total:\t" << msg->memory_total << "\tMB" << std::endl;
        show_str << "memory remain:\t" << msg->memory_available << "\tMB" << std::endl;
        show_str << "network sent:\t" << msg->net_sent << "\tMB" << std::endl;
        show_str << "network recv:\t" << msg->net_recv << "\tMB" << std::endl;
        return QString::fromStdString(show_str.str());
    }
private:
    rclcpp::Subscription<SystemStatus>::SharedPtr subscriber;
    std::shared_ptr<QLabel> label;
};

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    auto node = std::make_shared<SysStatusDisplay>("sys_status_display");
    std::thread spin_thread([&]()->void{
        rclcpp::spin(node);
        std::cout << "rcl loop terminate!" << std::endl;
    });
    // spin_thread.detach();
    app.exec();
    std::cout << "app closed" << std::endl;
    rclcpp::shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if(spin_thread.joinable()){
        spin_thread.join();
        std::cout << "wait for child thread terminate!" << std::endl;
    }
    else{
        std::cout << "some error!" << std::endl;
    }
    return 0;
}