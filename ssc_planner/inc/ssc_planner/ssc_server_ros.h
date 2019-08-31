/**
 * @file ssc_server_ros.h
 * @author HKUST Aerial Robotics Group
 * @brief planner server
 * @version 0.1
 * @date 2019-02
 * @copyright Copyright (c) 2019
 */
#ifndef _UTIL_SSC_PLANNER_INC_SSC_SERVER_ROS_H_
#define _UTIL_SSC_PLANNER_INC_SSC_SERVER_ROS_H_
#include "ros/ros.h"

#include <chrono>
#include <numeric>
#include <thread>

#include "semantic_map_manager/semantic_map_manager.h"
#include "semantic_map_manager/visualizer.h"
#include "ssc_planner/map_adapter.h"
#include "ssc_planner/ssc_planner.h"
#include "ssc_planner/ssc_visualizer.h"

#include "common/basics/colormap.h"
#include "common/basics/tic_toc.h"
#include "common/lane/lane.h"
#include "common/lane/lane_generator.h"
#include "common/trajectory/trajectory.h"
#include "common/visualization/common_visualization_util.h"
#include "moodycamel/atomicops.h"
#include "moodycamel/readerwriterqueue.h"

#include <sensor_msgs/Joy.h>
#include "tf/tf.h"
#include "tf/transform_datatypes.h"
#include "vehicle_msgs/ControlSignal.h"
#include "vehicle_msgs/encoder.h"
#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"

namespace planning {
class SscPlannerServer {
 public:
  using SemanticMapManager = semantic_map_manager::SemanticMapManager;

  struct Config {
    int kInputBufferSize{100};
  };

  SscPlannerServer(ros::NodeHandle nh, int ego_id);

  SscPlannerServer(ros::NodeHandle nh, double work_rate, int ego_id);

  void PushSemanticMap(const SemanticMapManager &smm);

  void Init();

  void Start();

 private:
  void PlanCycleCallback();

  void JoyCallback(const sensor_msgs::Joy::ConstPtr &msg);

  void Replan();

  void PublishData();

  void MainThread();

  Config config_;

  bool is_replan_on_ = false;
  bool is_map_updated_ = false;
  common::Trajectory executing_traj_;
  common::Trajectory next_traj_;

  SscPlanner planner_;
  SscPlannerAdapter map_adapter_;

  TicToc time_profile_tool_;
  decimal_t global_init_stamp_{0.0};

  common::VehicleControlSignal joy_ctrl_signal;

  // ros related
  ros::NodeHandle nh_;
  decimal_t work_rate_ = 20.0;
  int ego_id_;

  ros::Publisher ctrl_signal_pub_;
  ros::Publisher map_marker_pub_;
  ros::Publisher executing_traj_vis_pub_;
  ros::Subscriber joy_sub_;

  // input buffer
  moodycamel::ReaderWriterQueue<SemanticMapManager> *p_input_smm_buff_;
  SemanticMapManager last_smm_;
  semantic_map_manager::Visualizer *p_smm_vis_{nullptr};

  SscVisualizer *p_ssc_vis_{nullptr};
  int last_trajmk_cnt_{0};
};

}  // namespace planning

#endif  // _UTIL_SSC_PLANNER_INC_SSC_SERVER_ROS_H__