//! 未使用
//! 未使用
//! 未使用

#ifndef GENERAL_DESIGN_MODE_PUBLISHSER_HPP_
#define GENERAL_DESIGN_MODE_PUBLISHSER_HPP_
#include "event_manger.hpp"

// auto event_msg_centre = EventMsgCentre::GetSingleton();
static std::mutex publisher_mutex;
class Publishser
{

private:
    std::optional<MsgNode> topic_;
    Event event_id_;

public:
    std::string name_;
    void Init(const std::string publishser_name, Event id);
    void PutEventData(std::any data);
    inline void PublishNewTopic(const std::string topic_name) noexcept;
    inline void RemoveOldTopic(const std::string old_topic_name) noexcept;
};
void Publishser::Init(const std::string publishser_name, Event id)
{
    name_ = publishser_name;
    event_id_ = id;
    return;
}
inline void Publishser::PublishNewTopic(const std::string topic_name) noexcept
{
    topic_ = std::optional{MsgNode{topic_name, name_, EventData()}};
    topic_->data.id = event_id_;
    std::lock_guard<std::mutex> lock(publisher_mutex);
    // 告诉中心需要加入新的节点
    event_msg_centre->new_msg_node_ = topic_;
    return;
}

inline void Publishser::RemoveOldTopic(const std::string old_topic_name) noexcept
{
    std::lock_guard<std::mutex> lock(publisher_mutex);
    // 告诉中心有个节点要删除
    event_msg_centre->old_msg_node_name_ = old_topic_name;
    return;
}

void Publishser::PutEventData(std::any data)
{
    std::lock_guard<std::mutex> lock(publisher_mutex);
    // 数据填入自己msg_node
    topic_->data.datas.push(data);
    std::cout << name_ << "在话题" << topic_->topic_name
              << ":新添加了数据\n";
    return;
}

#endif // !GENERAL_DESIGN_MODE_PUBLISHSER_HPP_