#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <iostream>



int main(int argc, char** argv)
{

	ros::init(argc, argv, "first_publisher"); 								  //노드 선언. 노드이름은 first_publisher임
	ros::NodeHandle n;                    								      //노드핸들 선언. publisher, subscriber 객체 선언 시 필요함.
	std_msgs::Float64 msg; 								    			  //메세지 선언. std_msgs속 Float64 구조체의 객체를 'msgs'로 선언함.
	ros::Publisher publisher = n.advertise<std_msgs::Float64>("first_topic", 100); //Publisher 객체 선언. 다루고자 하는 토픽의 메세지형은 std_msgs::Float64이고, 토픽 이름은 first_topic 임.


	ros::Rate rate(100);                     //루프 레이트 설정하기. 100헤르츠로 돌릴 수 있음.


	double i = 0;

	while(ros::ok())						//ros 가 켜져있으면 true임.
	{
	
	i = i + 0.01;								//루프 한 번에 1씩 올라가는 i 라는 변수임.
	msg.data = i; 							//14번째 줄에서 선언했던 msg 문자형에다가 i를 집어넣음
	publisher.publish(msg);					//15번째 줄에서 선언했던 publisher 객체로 msg를 publish 함.
	ROS_INFO("Publising : %lf", i);			//터미널 용 printf

	ros::spinOnce();               //콜백함수들을 한 번 실행함. 여기 줄에서 publish 함.
	rate.sleep();                  //위에서 선언한 rate 만큼 쉼.
	}
	return 0;
}



