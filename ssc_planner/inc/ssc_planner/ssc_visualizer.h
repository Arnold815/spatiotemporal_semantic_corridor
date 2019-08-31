/**
 * @file ssc_visualizer.h
 * @author HKUST Aerial Robotics Group
 * @brief visualizer for the planner
 * @version 0.1
 * @date 2019-02
 * @copyright Copyright (c) 2019
 */
#ifndef _UTIL_SSC_PLANNER_INC_VISUALIZER_H_
#define _UTIL_SSC_PLANNER_INC_VISUALIZER_H_

#include <assert.h>
#include <iostream>
#include <vector>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#include "common/basics/basics.h"
#include "common/basics/semantics.h"
#include "common/primitive/frenet_primitive.h"
#include "common/state/frenet_state.h"
#include "common/state/state.h"
#include "common/visualization/common_visualization_util.h"

#include "ssc_planner/ssc_planner.h"

namespace planning {

class SscVisualizer {
 public:
  SscVisualizer(ros::NodeHandle nh, int node_id);
  ~SscVisualizer() {}

  void VisualizeDataWithStamp(const ros::Time &stamp,
                              const SscPlanner &planner);

 private:
  void VisualizeSscMap(const ros::Time &stamp, const SscMap *p_ssc_map);
  void VisualizeEgoVehicleInSscSpace(const ros::Time &stamp,
                                     const vec_E<Vec2f> &ego_vehicle_contour_fs,
                                     const decimal_t &ego_time);
  void VisualizeForwardTrajectoriesInSscSpace(
      const ros::Time &stamp, const vec_E<vec_E<common::FsVehicle>> &trajs,
      const SscMap *p_ssc_map);
  void VisualizeQpTrajs(const ros::Time &stamp,
                        const vec_E<common::BezierSpline<5, 2>> &trajs);
  void VisualizeSurroundingVehicleTrajInSscSpace(
      const ros::Time &stamp,
      const std::unordered_map<int, vec_E<common::FsVehicle>> &trajs,
      const SscMap *p_ssc_map);
  void VisualizeCorridorsInSscSpace(
      const ros::Time &stamp,
      const vec_E<common::DrivingCorridor3D> corridor_vec,
      const SscMap *p_ssc_map);
  void VisualizeSemanticVoxelSet(
      const ros::Time &stamp,
      const std::vector<common::AxisAlignedsCubeNd<int, 3>> &voxel_set,
      const SscMap *p_ssc_map);

  int last_traj_list_marker_cnt_ = 0;
  int last_surrounding_vehicle_marker_cnt_ = 0;

  ros::NodeHandle nh_;
  int node_id_;

  decimal_t start_time_;

  ros::Publisher ssc_map_pub_;
  ros::Publisher ego_vehicle_pub_;
  ros::Publisher forward_trajs_pub_;
  ros::Publisher sur_vehicle_trajs_pub_;
  ros::Publisher corridor_pub_;
  ros::Publisher qp_pub_;
  ros::Publisher semantic_voxel_set_pub_;

  int last_corridor_mk_cnt = 0;
  int last_qp_traj_mk_cnt = 0;
  int last_sur_vehicle_traj_mk_cnt = 0;
  int last_forward_traj_mk_cnt = 0;
  decimal_t marker_lifetime_{0.05};
};  // SscVisualizer

}  // namespace planning

#endif  // _UTIL_SSC_PLANNER_INC_VISUALIZER_H_