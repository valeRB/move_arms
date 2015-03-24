/*********************************************************************
* Software License Agreement (BSD License)
*
* Copyright (c) 2013, SRI International
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above
* copyright notice, this list of conditions and the following
* disclaimer in the documentation and/or other materials provided
* with the distribution.
* * Neither the name of SRI International nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/
/* Author: Sachin Chitta */
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
int main(int argc, char **argv)
{
    ros::init(argc, argv, "move_arms_test");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    spinner.start();
    /* This sleep is ONLY to allow Rviz to come up */
    //sleep(20.0);

    //sleep(5.0);
    // BEGIN_TUTORIAL
    //
    // Setup
    // ^^^^^
    //

    // The :move_group_interface:`MoveGroup` class can be easily
    // setup using just the name
    // of the group you would like to control and plan for.
    moveit::planning_interface::MoveGroup group_r("right_arm");
    moveit::planning_interface::MoveGroup group_l("left_arm");

    // We will use the :planning_scene_interface:`PlanningSceneInterface`
    // class to deal directly with the world.
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    // (Optional) Create a publisher for visualizing plans in Rviz.
    ros::Publisher display_publisher = node_handle.advertise<moveit_msgs::DisplayTrajectory>("/move_group/display_planned_path", 1, true);
    moveit_msgs::DisplayTrajectory display_trajectory;

    // Getting Basic Information
    // ^^^^^^^^^^^^^^^^^^^^^^^^^
    //
    // We can print the name of the reference frame for this robot.
    ROS_INFO("Reference frame: %s", group_r.getPlanningFrame().c_str());
    // We can also print the name of the end-effector link for this group.
    ROS_INFO("Reference frame end-eff right_arm: %s", group_r.getEndEffectorLink().c_str());
    ROS_INFO("Reference frame end-eff left_arm: %s", group_l.getEndEffectorLink().c_str());
    std::cout << "Right_arm \n" << std::endl;
    ROS_INFO("Current w: %f", group_r.getCurrentPose().pose.orientation.w); // 0.4599
    ROS_INFO("Current x: %f", group_r.getCurrentPose().pose.position.x); // 0.5853
    ROS_INFO("Current y: %f", group_r.getCurrentPose().pose.position.y); // -0.188
    ROS_INFO("Current z: %f", group_r.getCurrentPose().pose.position.z); //1.25
    std::cout << "Left_arm \n" << std::endl;
    ROS_INFO("Current w: %f", group_l.getCurrentPose().pose.orientation.w); // 0.4599
    ROS_INFO("Current x: %f", group_l.getCurrentPose().pose.position.x); // 0.5853
    ROS_INFO("Current y: %f", group_l.getCurrentPose().pose.position.y); // -0.188
    ROS_INFO("Current z: %f", group_l.getCurrentPose().pose.position.z); //1.25

    // Now, we call the planner to compute the plan
    // and visualize it.
    // Note that we are just planning, not asking move_group
    // to actually move the robot.
    moveit::planning_interface::MoveGroup::Plan my_plan;

    //    // Planning with Path Constraints
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //
    // Path constraints can easily be specified for a link on the robot.
    // Let's specify a path constraint and a pose goal for our group.
    // First define the path constraint.

    moveit_msgs::OrientationConstraint ocm;
    ocm.link_name =  group_r.getEndEffectorLink().c_str(); //"r_wrist_roll_link";
    ocm.header.frame_id = "base_link";
    ocm.orientation.w = 1.0;
    ocm.absolute_x_axis_tolerance = 0.1;
    ocm.absolute_y_axis_tolerance = 0.1;
    ocm.absolute_z_axis_tolerance = 0.1;
    ocm.weight = 1.0; //1.0
    // Now, set it as the path constraint for the group.
    moveit_msgs::Constraints right_constraints, left_constraints;
    right_constraints.orientation_constraints.push_back(ocm);
    // Use same values for ocm but change end-effector
    ocm.link_name =  group_l.getEndEffectorLink().c_str();
    left_constraints.orientation_constraints.push_back(ocm);
    group_r.setPathConstraints(right_constraints);
    group_l.setPathConstraints(left_constraints);

    geometry_msgs::Pose start_pose_r, start_pose_l;
    // Set start pose of right arm
    robot_state::RobotState start_state_r(*group_r.getCurrentState());
    start_pose_r.orientation.w = 1.0;
    start_pose_r.position.x = group_r.getCurrentPose().pose.position.x; // 0.55;
    start_pose_r.position.y = group_r.getCurrentPose().pose.position.y; // -0.05;
    start_pose_r.position.z = group_r.getCurrentPose().pose.position.z; // 0.8;
    const robot_state::JointModelGroup *joint_model_group = start_state_r.getJointModelGroup(group_r.getName()); //of group right_arm
    start_state_r.setFromIK(joint_model_group, start_pose_r);
    group_r.setStartState(start_state_r);

    // Set start pose of right arm
    robot_state::RobotState start_state_l(*group_l.getCurrentState());
    start_pose_l.orientation.w = 1.0;
    start_pose_l.position.x = group_l.getCurrentPose().pose.position.x; //
    start_pose_l.position.y = group_l.getCurrentPose().pose.position.y; //
    start_pose_l.position.z = group_l.getCurrentPose().pose.position.z; //
    joint_model_group = start_state_l.getJointModelGroup(group_l.getName()); //of group right_arm
    start_state_l.setFromIK(joint_model_group, start_pose_l);
    group_l.setStartState(start_state_l);

    // Set_target: we only change the value for y
    geometry_msgs::Pose target_pose1_right;
    target_pose1_right.orientation.w = 1.0; //group_r.getCurrentPose().pose.orientation.w;
    target_pose1_right.position.x = group_r.getCurrentPose().pose.position.x;
    target_pose1_right.position.y = group_r.getCurrentPose().pose.position.y;
    target_pose1_right.position.z = 0.7; // good height for the robot

    geometry_msgs::Pose target_pose1_left;
    target_pose1_left.orientation.w = 1.0; //group_r.getCurrentPose().pose.orientation.w;
    target_pose1_left.position.x = group_l.getCurrentPose().pose.position.x;
    target_pose1_left.position.y = group_l.getCurrentPose().pose.position.y;
    target_pose1_left.position.z = 0.7; // good height for the robot

    // Now we will plan to the earlier pose target from the new
    // start state that we have just created.
    for(int i=1; i<=5; i++)
    {
    group_r.setPoseTarget(target_pose1_right);
    group_l.setPoseTarget(target_pose1_left);
    bool success_r = group_r.plan(my_plan);
    ROS_INFO("Visualizing plan 3 right_arm %s",success_r?"":"FAILED");
    bool success_l = group_l.plan(my_plan);
    ROS_INFO("Visualizing plan 3 left_arm %s",success_l?"":"FAILED");
    /* Sleep to give Rviz time to visualize the plan. */
    sleep(3.0);
    }

    // Visualizing plans
    // ^^^^^^^^^^^^^^^^^
    // Now that we have a plan we can visualize it in Rviz. This is not
    // necessary because the group.plan() call we made above did this
    // automatically. But explicitly publishing plans is useful in cases that we
    // want to visualize a previously created plan.
//    if (1)
//    {
//    ROS_INFO("Visualizing plan 1 (again)");
//    display_trajectory.trajectory_start = my_plan.start_state_;
//    display_trajectory.trajectory.push_back(my_plan.trajectory_);
//    display_publisher.publish(display_trajectory);
//    /* Sleep to give Rviz time to visualize the plan. */
//    sleep(5.0);
//    }

//    // Moving to a pose goal
//    // ^^^^^^^^^^^^^^^^^^^^^
//    //
//    // Moving to a pose goal is similar to the step above
//    // except we now use the move() function. Note that
//    // the pose goal we had set earlier is still active
//    // and so the robot will try to move to that goal. We will
//    // not use that function in this tutorial since it is
//    // a blocking function and requires a controller to be active
//    // and report success on execution of a trajectory.
//    /* Uncomment below line when working with a real robot*/
//    /* group.move() */

//    // Planning to a joint-space goal
//    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//    //
//    // Let's set a joint space goal and move towards it. This will replace the
//    // pose target we set above.
//    //
//    // First get the current set of joint values for the group.
//    std::vector<double> group_variable_values;
//    group_r.getCurrentState()->copyJointGroupPositions(group_r.getCurrentState()->getRobotModel()->getJointModelGroup(group_r.getName()), group_variable_values);
////    // Now, let's modify one of the joints, plan to the new joint
////    // space goal and visualize the plan.
//    group_variable_values[1] = 0.5;
//    group_r.setJointValueTarget(group_variable_values);
//    moveit::planning_interface::MoveGroup::Plan my_plan;
//    bool success = group_r.plan(my_plan);
//    ROS_INFO("Visualizing plan 2 (joint space goal) %s",success?"":"FAILED");
////    /* Sleep to give Rviz time to visualize the plan. */
//    sleep(5.0);
//    if (1)
//    {
//        ROS_INFO("Visualizing plan 2 (again)");
//        display_trajectory.trajectory_start = my_plan.start_state_;
//        display_trajectory.trajectory.push_back(my_plan.trajectory_);
//        display_publisher.publish(display_trajectory);
//        /* Sleep to give Rviz time to visualize the plan. */
//        sleep(5.0);
//    }



    // When done with the path constraint be sure to clear it.
    //group.clearPathConstraints();


//    // Cartesian Paths
//    // ^^^^^^^^^^^^^^^
//    // You can plan a cartesian path directly by specifying a list of waypoints
//    // for the end-effector to go through. Note that we are starting
//    // from the new start state above. The initial pose (start state) does not
//    // need to be added to the waypoint list.
//    std::vector<geometry_msgs::Pose> waypoints;
//    geometry_msgs::Pose target_pose3 = start_pose2;
//    target_pose3.position.x += 0.2;
//    target_pose3.position.z += 0.2;
//    waypoints.push_back(target_pose3); // up and out
//    target_pose3.position.y -= 0.2;
//    waypoints.push_back(target_pose3); // left
//    target_pose3.position.z -= 0.2;
//    target_pose3.position.y += 0.2;
//    target_pose3.position.x -= 0.2;
//    waypoints.push_back(target_pose3); // down and right (back to start)
//    // We want the cartesian path to be interpolated at a resolution of 1 cm
//    // which is why we will specify 0.01 as the max step in cartesian
//    // translation. We will specify the jump threshold as 0.0, effectively
//    // disabling it.
//    moveit_msgs::RobotTrajectory trajectory;
//    double fraction = group.computeCartesianPath(waypoints,
//    0.01, // eef_step
//    0.0, // jump_threshold
//    trajectory);
//    ROS_INFO("Visualizing plan 4 (cartesian path) (%.2f%% acheived)",
//    fraction * 100.0);
//    /* Sleep to give Rviz time to visualize the plan. */
//    sleep(15.0);


//    // Adding/Removing Objects and Attaching/Detaching Objects
//    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//    // First, we will define the collision object message.
//    moveit_msgs::CollisionObject collision_object;
//    collision_object.header.frame_id = group.getPlanningFrame();
//    /* The id of the object is used to identify it. */
//    collision_object.id = "box1";
//    /* Define a box to add to the world. */
//    shape_msgs::SolidPrimitive primitive;
//    primitive.type = primitive.BOX;
//    primitive.dimensions.resize(3);
//    primitive.dimensions[0] = 0.4;
//    primitive.dimensions[1] = 0.1;
//    primitive.dimensions[2] = 0.4;
//    /* A pose for the box (specified relative to frame_id) */
//    geometry_msgs::Pose box_pose;
//    box_pose.orientation.w = 1.0;
//    box_pose.position.x = 0.6;
//    box_pose.position.y = -0.4;
//    box_pose.position.z = 1.2;
//    collision_object.primitives.push_back(primitive);
//    collision_object.primitive_poses.push_back(box_pose);
//    collision_object.operation = collision_object.ADD;
//    std::vector<moveit_msgs::CollisionObject> collision_objects;
//    collision_objects.push_back(collision_object);
//    // Now, let's add the collision object into the world
//    ROS_INFO("Add an object into the world");
//    planning_scene_interface.addCollisionObjects(collision_objects);
//    /* Sleep so we have time to see the object in RViz */
//    sleep(2.0);
//    // Planning with collision detection can be slow. Lets set the planning time
//    // to be sure the planner has enough time to plan around the box. 10 seconds
//    // should be plenty.
//    group.setPlanningTime(10.0);
//    // Now when we plan a trajectory it will avoid the obstacle
//    group.setStartState(*group.getCurrentState());
//    group.setPoseTarget(target_pose1);
//    success = group.plan(my_plan);
//    ROS_INFO("Visualizing plan 5 (pose goal move around box) %s",
//    success?"":"FAILED");
//    /* Sleep to give Rviz time to visualize the plan. */
//    sleep(10.0);
//    // Now, let's attach the collision object to the robot.
//    ROS_INFO("Attach the object to the robot");
//    group.attachObject(collision_object.id);
//    /* Sleep to give Rviz time to show the object attached (different color). */
//    sleep(4.0);
//    // Now, let's detach the collision object from the robot.
//    ROS_INFO("Detach the object from the robot");
//    group.detachObject(collision_object.id);
//    /* Sleep to give Rviz time to show the object detached. */
//    sleep(4.0);
//    // Now, let's remove the collision object from the world.
//    ROS_INFO("Remove the object from the world");
//    std::vector<std::string> object_ids;
//    object_ids.push_back(collision_object.id);
//    planning_scene_interface.removeCollisionObjects(object_ids);
//    /* Sleep to give Rviz time to show the object is no longer there. */
//    sleep(4.0);
//    // Dual-arm pose goals
//    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//    // First define a new group for addressing the two arms. Then define
//    // two separate pose goals, one for each end-effector. Note that
//    // we are reusing the goal for the right arm above
//    moveit::planning_interface::MoveGroup two_arms_group("arms");
//    two_arms_group.setPoseTarget(target_pose1, "r_wrist_roll_link");
//    geometry_msgs::Pose target_pose2;
//    target_pose2.orientation.w = 1.0;
//    target_pose2.position.x = 0.7;
//    target_pose2.position.y = 0.15;
//    target_pose2.position.z = 1.0;
//    two_arms_group.setPoseTarget(target_pose2, "l_wrist_roll_link");
//    // Now, we can plan and visualize
//    moveit::planning_interface::MoveGroup::Plan two_arms_plan;
//    two_arms_group.plan(two_arms_plan);
//    sleep(4.0);

    // END_TUTORIAL
    ros::shutdown();
    return 0;
}
