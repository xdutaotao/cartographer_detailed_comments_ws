/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
#define CARTOGRAPHER_MAPPING_IMU_TRACKER_H_

#include "Eigen/Geometry"
#include "cartographer/common/time.h"

namespace cartographer {
namespace mapping {

// Keeps track of the orientation using angular velocities and linear
// accelerations from an IMU. Because averaged linear acceleration (assuming
// slow movement) is a direct measurement of gravity, roll/pitch does not drift,
// though yaw does.
// 使用来自IMU的角速度和线性加速度跟踪方向
// 由于平均线性加速度(假设缓慢移动)是重力的直接量度, 因此, 横摆/俯仰不会漂移, 尽管偏航会漂移

/**
 * @brief 
 * ImuTracker 的主要作用是根据 IMU的角速度来预测姿态,
 * 并根据IMU的线加速度来确定重力的方向, 并使用重力的方向来对姿态进行校准
 */
class ImuTracker {
 public:
  ImuTracker(double imu_gravity_time_constant, common::Time time);

  // Advances to the given 'time' and updates the orientation to reflect this.
  void Advance(common::Time time); // 提前预测time对应的位姿

  // 加入IMU的角速度和线加速度
  // Updates from an IMU reading (in the IMU frame).
  void AddImuLinearAccelerationObservation(
      const Eigen::Vector3d& imu_linear_acceleration);
  void AddImuAngularVelocityObservation(
      const Eigen::Vector3d& imu_angular_velocity);

  // Query the current time.
  // 获取上一次预测位姿的时间戳
  common::Time time() const { return time_; }

  // Query the current orientation estimate.
  // 查询当前估计出的姿态
  Eigen::Quaterniond orientation() const { return orientation_; }

 private:
  const double imu_gravity_time_constant_;      // 重力时间常数 9.8 ?
  common::Time time_;                           // 上一次预测位姿的时间戳
  common::Time last_linear_acceleration_time_;  // 上一次加入线加速度的时间戳
  Eigen::Quaterniond orientation_;              // 上一次预测的位姿
  Eigen::Vector3d gravity_vector_;              // 重力方向，向量
  Eigen::Vector3d imu_angular_velocity_;        // IMU的角速度
};

}  // namespace mapping
}  // namespace cartographer

#endif  // CARTOGRAPHER_MAPPING_IMU_TRACKER_H_
