#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/JointState.h"
#include <iostream>
#include <cmath>
#include <geometry_msgs/Twist.h>
// #include "test/SetPosition.h"
#include "dynamixel_workbench_msgs/DasomDynamixel.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense> 

double effort_1 = 0;
double torque_const = 0.044;
double torque_1 = 0;

double angle_1 = 0;
double Tau_gravity = 0;
double WL = 0.015;

Eigen::Vector2d LPF_torque_1;

/*void hapticCallback(const geometry_msgs::Twist &msg)
{
		joint1 = msg.linear.x;
		joint2 = msg.linear.y;
		joint3 = msg.linear.z;
		joint4 = msg.angular.x;
		joint5 = msg.angular.y;
		joint6 = msg.angular.z;
}
*/
void sensor_callback(const dynamixel_workbench_msgs::DasomDynamixel::ConstPtr &msg)
{
	effort_1 = msg->effort.at(0);
	torque_1 = effort_1 * torque_const;

	angle_1 = msg->position.at(0);


}



int main(int argc, char** argv)
{

	ros::init(argc, argv, "Goal_Joint_Position");
	ros::NodeHandle n;
	ros::Publisher publisher = n.advertise<dynamixel_workbench_msgs::DasomDynamixel>("/dasom/goal_dynamixel_position", 100);
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
	ros::Subscriber sub = n.subscribe("/dasom/joint_states", 10, &sensor_callback);

//	ros::Subscriber hapticCallback_sub = n.subscribe("/now_haptic_endEffector_publisher", 10, hapticCallback);
 
	geometry_msgs::Twist cmd_;

	ros::Rate rate(250);


	float sine = 0;
	float i = 0;

	double Amplitude = 0;
	double period = 0;
	
	ROS_INFO("Amplitude : ");
	std::cin >> Amplitude;

	ROS_INFO("period : ");
	std::cin >> period;

	while(ros::ok())
	{

	//------------------------------
	//LPF
	LPF_torque_1[0] = WL * torque_1 + (1 - WL) * LPF_torque_1[1];
	LPF_torque_1[1] = LPF_torque_1[0];



	i = i + 0.004;

	sine = - Amplitude * cos(3.141592 * i / period) + Amplitude;

	//-------------------------------------------------------
	//중력에 의한 토크를 구해봤어요
	Tau_gravity = 0.09317 * cos(angle_1) * 0.122 - 0.004;   //0.005 : 보정값


		dynamixel_workbench_msgs::DasomDynamixel cmd_fromHaptic;
		cmd_fromHaptic.position.push_back(sine);
		// cmd_.linear.x = sine;
		// cmd_.linear.y = sine + 0.05;
		// cmd_.linear.z = sine - 0.05;	

		cmd_.linear.x = LPF_torque_1[0];
		cmd_.linear.y = Tau_gravity;
		cmd_.linear.z = torque_1;

//		publisher.publish(cmd_fromHaptic);
		pub.publish(cmd_);
		ros::spinOnce();
		rate.sleep();


		ROS_INFO("LPF_torque_1 : %lf", LPF_torque_1[0]);
		ROS_INFO("Tau_Gravity : %lf", Tau_gravity);
		ROS_INFO("Error gravity : %lf", Tau_gravity - LPF_torque_1[0]);
		ROS_INFO("-------------------");







	}

	return 0;


}