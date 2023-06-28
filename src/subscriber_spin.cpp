#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <iostream>

void first_callback(const std_msgs::Float64 &msg)     //받는 토픽은 std_msgs::Float64 형임. msg 라는 객체로 받음.
{
	ROS_INFO("Subscribed : %lf", msg.data);           //msg객체의 data를 printf함.
}


int main(int argc, char** argv)
{

	ros::init(argc, argv, "first_subscriber_spinOnce");   //노드 선언. 노드이름은 first_publisher임
	ros::NodeHandle n;                          //노드핸들 선언. publisher, subscriber 객체 선언 시 필요함.
	ros::Subscriber subscriber = n.subscribe("first_topic", 1000, first_callback);  //subscriber 객체 선언. 다루고자 하는 토픽 이름은 first_topic 이며, 그 때마다 first_callback이 실행 됨.



	ros::spin();


	
	return 0;


}

