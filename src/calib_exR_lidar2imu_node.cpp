#include <ros/ros.h>
#include <iostream>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <queue>
#include "calibExRLidar2Imu.h"

using namespace std;

queue<sensor_msgs::PointCloud2ConstPtr> lidar_buffer;
queue<sensor_msgs::ImuConstPtr> imu_buffer;

void lidarCallback(const sensor_msgs::PointCloud2ConstPtr &msg)
{
    lidar_buffer.push(msg);
}

void imuCallback(const sensor_msgs::ImuConstPtr &msg)
{
    imu_buffer.push(msg);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "calib_exR_lidar2imu_node");
    ros::NodeHandle nh, pnh("~");

    ros::Subscriber lidar_sub = nh.subscribe<sensor_msgs::PointCloud2>("/cloud", 1000, lidarCallback);
    ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/imu", 10000, imuCallback);

    CalibExRLidarImu caliber;

    // add data
    ros::Rate loop_rate(200);
    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO_STREAM_THROTTLE(5.0, "lidar buffer size " << lidar_buffer.size() << ", imu buffer size " << imu_buffer.size()); // monitor

        // add lidar data
        while (lidar_buffer.size() != 0)
        {
            CloudT::Ptr cloud(new CloudT);
            pcl::fromROSMsg(*(lidar_buffer.front()), *cloud);
            LidarData data;
            data.cloud = cloud;
            data.stamp = lidar_buffer.front()->header.stamp.toSec();
            caliber.addLidarData(data);
            lidar_buffer.pop();
        }

        // add imu data
        while (imu_buffer.size() != 0)
        {
            ImuData data;
            data.acc = Eigen::Vector3d(imu_buffer.front()->linear_acceleration.x,
                                       imu_buffer.front()->linear_acceleration.y,
                                       imu_buffer.front()->linear_acceleration.z);
            data.gyr = Eigen::Vector3d(imu_buffer.front()->angular_velocity.x,
                                       imu_buffer.front()->angular_velocity.y,
                                       imu_buffer.front()->angular_velocity.z);
            data.rot = Eigen::Quaterniond(imu_buffer.front()->orientation.w,
                                          imu_buffer.front()->orientation.x,
                                          imu_buffer.front()->orientation.y,
                                          imu_buffer.front()->orientation.z);
            data.stamp = imu_buffer.front()->header.stamp.toSec();
            caliber.addImuData(data);
            imu_buffer.pop();
        }

        loop_rate.sleep();
    }

    // calib
    Eigen::Vector3d rpy = caliber.calib();
    cout << "result extrinsic R -> " << rpy[0] << " " << rpy[1] << " " << rpy[2] << endl;

    return 0;
}