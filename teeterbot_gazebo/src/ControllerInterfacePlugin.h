#ifndef CONTROLLERINTERFACEPLUGIN_H
#define CONTROLLERINTERFACEPLUGIN_H

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

#include "DcMotorSim.h"

namespace gazebo
{

class ControllerInterfacePlugin : public ModelPlugin
{
public:
    ControllerInterfacePlugin();
    virtual ~ControllerInterfacePlugin();

protected:
    virtual void Load ( physics::ModelPtr model, sdf::ElementPtr sdf );
    virtual void Reset();

private:
    void OnUpdate ( const common::UpdateInfo &info );
    void data100Cb ( const ros::TimerEvent &event );
    void recvMotorVoltage ( const std_msgs::Float64ConstPtr &msg, int side );
    void getEuler ( double &roll, double &pitch, double &yaw );

    // ROS
    ros::NodeHandle *n_;
    ros::Publisher pub_left_encoder_;
    ros::Publisher pub_right_encoder_;
    ros::Publisher pub_fallen_over_;
    ros::Subscriber sub_left_voltage_;
    ros::Subscriber sub_right_voltage_;
    ros::Timer data_100Hz_timer_;
    tf::TransformBroadcaster broadcaster_;

    // Gazebo
    event::ConnectionPtr update_connection_;
    physics::ModelPtr model_;
    physics::JointPtr left_wheel_joint_;
    physics::JointPtr right_wheel_joint_;
    physics::LinkPtr left_wheel_link_;
    physics::LinkPtr right_wheel_link_;
    physics::LinkPtr body_link_;

    // DC motor simulation instances
    teeterbot_gazebo::DcMotorSim *left_motor_;
    teeterbot_gazebo::DcMotorSim *right_motor_;

    // Status properties
    bool fallen_over_;
    double left_voltage_;
    double right_voltage_;
    double fallen_over_stamp_;

    // SDF parameters
    bool pub_ground_truth_;
    bool auto_reset_orientation_;
    double auto_reset_delay_;
};

GZ_REGISTER_MODEL_PLUGIN ( ControllerInterfacePlugin )

}

#endif // CONTROLLERINTERFACEPLUGIN_H
