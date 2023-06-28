#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include <iostream>
#include <geometry_msgs/Twist.h>
#include <dynamixel_workbench_msgs/DasomDynamixel.h>

double command_position_fromGUI = 0;
double position_dot = 0;
double velocity_command = 0;
double position_gain = 0;

double P_gain = 10;
double I_gain = 1;
double D_gain = 0.1;

double error_i = 0;
double error_d = 0;
double error_p = 0;
double error_p_i = 0;

double time_f = 0;
double time_i = 0;
double time_loop = 0;

double measured_position = 0;
double measured_velocity = 0;


void commandcallback(const dynamixel_workbench_msgs::DasomDynamixel::ConstPtr &msg)
{
	command_position_fromGUI = msg->position.at(0);
}


void jointcallback(const sensor_msgs::JointState::ConstPtr &msg)
{
	measured_position = msg->position.at(0);
	measured_velocity = msg->velocity.at(0);



}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "Admittance_Test_Only_K");
	ros::NodeHandle n;
	ros::Subscriber CommandSub = n.subscribe("/dasom/reference_position", 10, commandcallback);  // Command From rqt
	ros::Publisher Commandpub = n.advertise<geometry_msgs::Twist>("/dasom/goal_dynamixel_position", 100); // Final Angle Command
	ros::Subscriber JointSub = n.subscribe("/dasom/joint_states", 10, jointcallback); // Effort, Position, Velocity
	ros::Publisher forPub = n.advertise<geometry_msgs::Twist>("/dasom/subscribedCommand", 100); // 왜 position[0]는 나오지 않ㄴ는 것인가...


	ros::Rate rate(250);

	geometry_msgs::Twist twist;
	geometry_msgs::Twist command;

	time_i = ros::Time::now().toSec();

	while(ros::ok())
	{
	time_f = ros::Time::now().toSec();
	time_loop = time_f - time_i;
	time_i = ros::Time::now().toSec();

	error_p = command_position_fromGUI - measured_position;
	error_i = error_i + error_p * time_loop;
	if (error_p - error_p_i != 0) error_d = (error_p - error_p_i) / time_loop;
	error_p_i = error_p;



	position_gain = P_gain * error_p + I_gain * error_i + D_gain * error_d;


	twist.linear.x = position_gain;
	twist.angular.z = 0;
	ROS_INFO("error_p=%lf, error_d=%lf, error_i=%lf, time_loop=%lf", error_p, error_d, error_i, time_loop);
	ROS_INFO("command : %lf, measured : %lf", command_position_fromGUI, measured_position);
	ROS_INFO("------");
	
	Commandpub.publish(twist);
	command.linear.x = command_position_fromGUI;
	forPub.publish(command);

	ros::spinOnce();
	rate.sleep();

	}
	return 0;



}