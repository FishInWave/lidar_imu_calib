# lidar_imu_calib

### overview

when develop slam based on 3D lidar, we often use imu to provide priori for matching algorithm(icp, ndt), so the transform between lidar and imu need to be calibrated.For matching algorithm, attitude in transfom is more important than position in transform, and position often be set to 0. So this repo concentrate on calibrate attitude component in transform between lidar and imu.

### prerequisite 

- [ROS](http://wiki.ros.org/kinetic/Installation/Ubuntu)
### compile
```
mkdir -p catkin_ws/src   
cd catkin_ws/src
git clone https://github.com/chennuo0125-HIT/lidar_imu_calib.git
cd ..
catkin_make -DCATKIN_WHITELIST_PACKAGES="ndt_omp;lidar_imu_calib"
```
### run step

1. use rosbag tool record imu and lidar data

   ```
   rosbag record /imu /lidar_points
   ```

2. config launch file

   ```
   lidar_topic: lidar data topic name
   imu_topic: imu data topic name
   bag_file: *.bag file record imu and lidar data topic
   ```


3. start

   ```
   roslaunch lidar_imu_calib calib_exR_lidar2imu.launch
   ```
### reference
[https://blog.csdn.net/weixin_37835423/article/details/110672571](https://blog.csdn.net/weixin_37835423/article/details/110672571)

### 修改
1. 单位为rad，激光点云数据类型建议为XYZI，若为rslidar的XYZIRT类型，建议使用rs_to_velodyne播放包并进行重新录制。
2. 删除了该仓库携带的ndt_omp，并修改了相关文件，使其链接catkin中的ndt_omp，避免重复编译
3. 编译时可能出现LZ4冲突定义问题，该问题是系统有两个LZ4导致的，解决方法见于https://github.com/ethz-asl/lidar_align/issues/16

### 输出示例
[ INFO] [1644326710.583261166]: add lidar msg ......

[ INFO] [1644326715.667613410]: add lidar msg ......

[ INFO] [1644326720.904714194]: add lidar msg ......

[ INFO] [1644326726.136801956]: add lidar msg ......

[ INFO] [1644326731.392233087]: add lidar msg ......

total lidar buffer size 999, imu buffer size 39375

constraints size 998

constraints size 998

result euler angle(RPY) : -0.00462066 0.0258202 -0.113875

result extrinsic rotation matrix : 

  0.993192   0.113509  0.0261749
 -0.113591   0.993526 0.00165715
-0.0258173 -0.0046191   0.999656

   
