#include <ros/ros.h>
#include <geometry_msgs/Twist.h> //guardar los valores de x, y, y theta
#include <geometry_msgs/Pose2D.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "gazebo_msgs/LinkStates.h"
#include "std_msgs/Float32.h"

double PI = 3.14159265; 
using namespace std; 

geometry_msgs::Pose2D start_pose; //start pose es la pose actual del auto
geometry_msgs::Pose2D target_pose;
float vel, gam;
std_msgs::Float32 msgVel;
std_msgs::Float32 msgGamma;
 
double rate_hz = 10;
double k_alpha = 8, k_ro = 3, k_beta = -3;



//Get the starting pose of the car in simulator.
void getStartPose(const geometry_msgs::Pose2D& startPose){
	start_pose.x = startPose.x;
	start_pose.y = startPose.y;
	start_pose.theta = startPose.theta; 
}

//Get from user the objective pose.
void getTargetPose(){
	float x,y, theta;
	cout << "Dame la posición en x: " << endl;
	cin >> x; 
	cout << "Dame la posición en y: " << endl;
	cin >> y;
	cout << "Dame el ángulo: " << endl;
	cin >> theta; 
	target_pose.x = x; 
	target_pose.y = y;
	theta = theta *(PI/180); 
	target_pose.theta = theta; 
}

void toVelocitiesConvertion(geometry_msgs::Pose2D startPose, geometry_msgs::Pose2D targetPose){
	double ro, alpha, beta;
	double x0 = startPose.x , y0 = startPose.y , theta0 = startPose.theta;
	double x1 = targetPose.x , y1 = targetPose.y , theta1 = targetPose.theta;
	ro = sqrt(pow(x1-x0,2.0) + pow(y1-y0,2.0));
	alpha = atan((y1-y0)/(x1-x0)) * (180 / PI) - theta0; 
	beta = -alpha - theta0;

	vel = k_ro * ro; 
	gam = k_alpha * alpha + k_beta * beta;
	ROS_INFO_STREAM(vel);

}

bool isTargetClose(geometry_msgs::Pose2D startPose, geometry_msgs::Pose2D targetPose){
	bool close = false; 
	if((abs(startPose.x - targetPose.x) <= 1) && (abs(startPose.y - targetPose.y) <= 1) && (abs(startPose.theta - targetPose.theta) <= 0.8)){
		close = true;
	}
	return close;
}

void publishControl()
{	
	if(isTargetClose){
		msgVel.data = 0;
		msgGamma.data = 0;
	}
	msgVel.data = vel;
	msgGamma.data = gam;

}
void toCoordinatesConversion();

int main(int argc, char **argv){
	ros::init(argc, argv, "car_trayectory_node");
	ros::NodeHandle nh;
	ROS_INFO_STREAM("Car trayectory node inicialized");


	ros::Publisher pubCarPose = nh.advertise <geometry_msgs::Pose2D>("/robot/pose",rate_hz);
	//Nodos de velocidad y volantes: manual_control/steering
	//--/AutoNOMOS_mini				/manual_control/velocity

	ros::Subscriber sub_carVel = nh.subscribe("/robot/pose",1000, &getStartPose);
	//NOdos de publicación de velocidades
	ros::Publisher give_carSpeed = nh.advertise<std_msgs::Float32>("/AutoNOMOS_mini/manual_control/velocity", rate_hz);
	ros::Publisher give_carSteering = nh.advertise<std_msgs::Float32>("/AutoNOMOS_mini/manual_control/steering", rate_hz);

	double time = 0; 

	getTargetPose();
	 //define the rate
	ros::Rate rate(rate_hz);
	while(ros::ok()){
		int i = 1;
			ROS_INFO_STREAM("Robot Position:"
			<<" X="<<start_pose.x
			<<",Y="<<start_pose.y
			<<",W="<<start_pose.theta);
		ROS_INFO_STREAM("Target position:"
			<<" X="<<target_pose.x
			<<",Y="<<target_pose.y
			<<",W="<<target_pose.theta);

			if(isTargetClose){
				msgVel.data = 0;
				msgGamma.data = 0;
			}
		toVelocitiesConvertion(start_pose,target_pose);

		//Publicar velocidad angular y lineal inicial 
		publishControl();
		give_carSpeed.publish(msgVel);
		give_carSteering.publish(msgGamma);

		ros::spinOnce();
		rate.sleep();
	}
return 0;
}
