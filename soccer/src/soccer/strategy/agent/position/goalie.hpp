#pragma once

#include <cmath>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <spdlog/spdlog.h>

#include <rj_msgs/action/robot_move.hpp>
#include <rj_msgs/msg/empty_motion_command.hpp>
#include <rj_msgs/msg/goalie_idle_motion_command.hpp>
#include <rj_msgs/msg/intercept_motion_command.hpp>
#include <rj_msgs/msg/path_target_motion_command.hpp>

#include "planning/planner/intercept_planner.hpp"
#include "position.hpp"
#include "rj_common/time.hpp"
#include "rj_geometry/geometry_conversions.hpp"
#include "rj_geometry/point.hpp"

namespace strategy {

/*
 * The Goalie position handles goalie behavior: blocking shots, passing to teammates, and clearing
 * the ball.
 */
class Goalie : public Position {
public:
    Goalie(int r_id);
    ~Goalie() override = default;

private:
    // temp
    int send_idle_ct_ = 0;

    std::optional<RobotIntent> derived_get_task(RobotIntent intent) override;

    /*
     * @return true if ball is heading towards goal at some minimum speed threshold
     */
    bool shot_on_goal_detected(WorldState* world_state);
};

}  // namespace strategy