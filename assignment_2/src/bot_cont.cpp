#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "assignment_2/Inc_dec.h"
#include <sstream>
#include <array>

#define DIST_MIN 0.5
#define MAX_AG 720
#define ANGL_RG_MAX 520
#define ANGL_RG_MIN 200
#define VELOCIDAD 1.5
#define INC 0.25

// 721 elementss in vector ranges

ros::Publisher pub1;
float mult = 1;	

bool front_view(float rg[])
{
	/*
	Funtion to see if there are any near obstacles in front of the robot
	
	Args:
		rg[] (float): vector that contains the distance of the obstacles to the robot in a 180 degree range
	
	Returns:
		true: if obstacles are detected
		falae: if no obstacles are detected
	*/
	
	int i = ANGL_RG_MIN;
	bool obs = false;
	while ((i <= ANGL_RG_MAX)&&(obs == false)){
		if (rg[i] < DIST_MIN)
			obs = true;
		i++;
	}
	return obs;
}



void robotCallBack(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	/*
	Callback subscribing to the topic /base_scan. It will change the velocity of the robot in order to 
	automatically drive it in the circuit 
	
	Args:
		msg (struct): messages on the topic of /base_scan
	*/

	geometry_msgs::Twist vel;
		
	// Aquí es donde se viene toda la logica detras del movimiento del robot
	
	float rang[msg->ranges.size()];
	for (int i=0; i<msg->ranges.size();i++)
		rang[i] = msg->ranges[i];
	
	bool fv = front_view(rang);
	bool giro = true;
	int a;	
		
	if (fv){	// if obstacles ahead
	
		ROS_INFO("Watch the wall!\n");
		
		if (giro){	// turns
			giro = false;
			vel.linear.x = 0.0;
			if (msg->ranges[0] < msg->ranges[MAX_AG])	
				a = 1;	
			else
				a = -1;
		}
		vel.angular.z = a*VELOCIDAD/2;
		
	}else{		// if no obstacles ahead
		giro = true;
		ROS_INFO("Avanti\n");
		vel.linear.x = mult*VELOCIDAD;
		vel.angular.z = 0.0;
	}
	pub1.publish(vel);	// publish the new velocity
}


bool options (assignment_2::Inc_dec::Request &req, assignment_2::Inc_dec::Response &res)
{
	/*
	Callback of the service /inc_dec. It will increase or decrease the velocity of the robot based on
	the user's input
	
	Args: 
		req (int): request of the service
		res (int): response of the service
	*/
	
	switch (req.option)
	{
		case 1:	// Increase the speed
			mult = mult + INC;
			break;
		case 2:	// Decrease the speed
			mult = mult - INC;
			if (mult<0)
				mult = 0;
			break;			
	}

	return true;
}


int main (int argc, char **argv)
{
	// Main code
	
	// Initialize ros node and its node handle
	ros::init(argc, argv, "bot_controller");
	ros::NodeHandle nh;
	
	// Subscriber to the topic /base_scan
	ros::Subscriber sub = nh.subscribe("/base_scan", 1, robotCallBack);
	
	// Publisher to the topci /cmd_vel
	pub1 = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	
	// Server of /inc_dec
	ros::ServiceServer service = nh.advertiseService("/inc_dec", options);
	
	ros::spin();
	
	return 0;
}
