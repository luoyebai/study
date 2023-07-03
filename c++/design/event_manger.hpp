//! 未使用
//! 未使用
//! 未使用

#ifndef GENERAL_DESIGN_MODE_EVENT_MANGER_HPP_
#define GENERAL_DESIGN_MODE_EVENT_MANGER_HPP_

// std
#include <any>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>

// 事件
enum Event { IMAGE_ACQUISTION, DETECT, SOLVER, SERIAL, RECORD };

// 事件名字
const std::string event_name[5]{
    "image_acquistion", // 图像采集
    "detect",           // 识别
    "solver",           // 解算
    "serial",           // 通信
    "record"            // 记录
};

// 事件的数据
struct EventData {
    // using Error = std::runtime_error;
    Event id;
    std::queue<std::any> datas;
    // Error error;
    const std::string name = event_name[id];
    EventData &operator=(const EventData &event_data) {
        return const_cast<EventData &>(event_data);
    };
};

// 消息节点
struct MsgNode {
    // 节点名字
    std::string topic_name;
    // 发布者名字
    std::string pub_name;
    // 数据信息
    EventData data;
};

class EventMsgCentre {
  private:
    static std::mutex msg_mutex_;
    // 消息队列
    static std::unordered_map<std::string, EventData> msg_nodes_;
    // 信息处理---包括发布，删除发布，订阅，取消订阅

  public:
    // 用来存储需要加入的新节点
    static std::optional<MsgNode> new_msg_node_;
    // 用来存储需要删除的老节点
    static std::optional<std::string> old_msg_node_name_;
    // 新的订阅
    static std::optional<std::string> new_subscribe_topic_;
    // 旧的订阅
    static std::optional<std::string> old_subscribe_topic_name_;
    // 处理事件
    static void EventProcess();

    static std::shared_ptr<EventMsgCentre> GetSingleton();
};

// 定义静态变量ODR
std::mutex EventMsgCentre::msg_mutex_;
std::unordered_map<std::string, EventData> EventMsgCentre::msg_nodes_;
std::optional<MsgNode> EventMsgCentre::new_msg_node_ = std::nullopt;
std::optional<std::string> EventMsgCentre::old_msg_node_name_ = std::nullopt;
std::optional<std::string> EventMsgCentre::new_subscribe_topic_ = std::nullopt;
std::optional<std::string> EventMsgCentre::old_subscribe_topic_name_ =
    std::nullopt;

void EventMsgCentre::EventProcess() {
    std::cout << "事件中心初始化成功\n";
    while (1) {
        // 添加节点
        if (new_msg_node_) {
            std::lock_guard<std::mutex> lock(msg_mutex_);
            msg_nodes_[new_msg_node_->topic_name] = new_msg_node_->data;
            new_msg_node_ = std::nullopt;
            std::cout << new_msg_node_->pub_name
                      << "添加了新的节点:" << new_msg_node_->topic_name << "\n";
        }
        // 删除节点
        if (old_msg_node_name_) {
            std::lock_guard<std::mutex> lock(msg_mutex_);
            msg_nodes_.erase(old_msg_node_name_.emplace());
            old_msg_node_name_ = std::nullopt;
            std::cout << new_msg_node_->pub_name
                      << "删除了节点:" << new_msg_node_->topic_name << "\n";
        }
        //
        if (new_subscribe_topic_)
            ;
        //
        if (old_subscribe_topic_name_)
            ;
    }
    return;
}

static std::mutex event_manger_mutex;
static std::shared_ptr<EventMsgCentre> event_msg_centre = nullptr;

std::shared_ptr<EventMsgCentre> EventMsgCentre::GetSingleton() {
    if (event_msg_centre == nullptr) {
        std::lock_guard<std::mutex> lock(event_manger_mutex);
        if (event_msg_centre == nullptr) {
            auto temp = std::shared_ptr<EventMsgCentre>(new EventMsgCentre());
            event_msg_centre = temp;
        }
    }
    return event_msg_centre;
}

#endif // !GENERAL_DESIGN_MODE_EVENT_MANGER_HPP_
