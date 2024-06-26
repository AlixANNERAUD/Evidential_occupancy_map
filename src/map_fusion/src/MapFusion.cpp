#include "EvidentialGrid.hpp"
#include <octomap_msgs/conversions.h>

void EvidentialGrid::publish()
{
    octomap_msgs::Octomap global_eogm_msg;
    if (!octomap_msgs::fullMapToMsg(this->global_eogm.getOctomap(), global_eogm_msg))
    {
        ROS_ERROR("Failed to serialize octomap");
    }

    global_eogm_msg.header.frame_id = "map";

    this->global_eogm_publisher.publish(global_eogm_msg);
}

void EvidentialGrid::fuse()
{
    static bool first_time = true;

    auto total_fuse = std::chrono::high_resolution_clock::now();

    // Set the origin of the global EOGM
    if (first_time)
    {
        auto origin_x = this->current_odometry.pose.pose.position.x - (this->global_eogm.getRealWidth() / 2);
        auto origin_y = this->current_odometry.pose.pose.position.y - (this->global_eogm.getRealHeight() / 2);
        
        
        ROS_INFO("Setting origin to (%f, %f)", origin_x, origin_y);
        this->global_eogm.setOrigin(origin_x, origin_y);
        first_time = false;
    }

    this->local_eogm.setOrigin(this->current_odometry.pose.pose.position.x, this->current_odometry.pose.pose.position.y);

    this->global_eogm.fuse(this->local_eogm);
}
