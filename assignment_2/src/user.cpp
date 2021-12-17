#include "ros/ros.h"
#include "assignment_2/Inc_dec.h"
#include "std_srvs/Empty.h"
#include <termios.h>
#include <signal.h>
#include <stdio.h>

#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71

int kfd = 0;
struct termios cooked, raw;

ros::ServiceClient client_vel;
ros::ServiceClient client_reset_pos;


void quit(int sig)
{
	/*
	Function to stop the user interface node using ctrl+c on the console
	*/
	
	tcsetattr(kfd, TCSANOW, &cooked);
 	ros::shutdown();
  	exit(0);
}


void keypressed ()
{  
	/*
	Function to tell which key is pressed
	*/
	
	char c;
   	assignment_2::Inc_dec o;
   	std_srvs::Empty r;
 
   	// get the console in raw mode                                                              
   	tcgetattr(kfd, &cooked);
   	memcpy(&raw, &cooked, sizeof(struct termios));
   	raw.c_lflag &=~ (ICANON | ECHO);
   	// Setting a new line, then end of file                         
   	raw.c_cc[VEOL] = 1;
   	raw.c_cc[VEOF] = 2;
   	tcsetattr(kfd, TCSANOW, &raw);
 
   	puts("User interface");
   	puts("---------------------------");
   	puts("Use arrows keys to increase (up) / decrease (down) the velocity or reset (left) the robot position.");
 
 
   	for(;;){
     		// get the next event from the keyboard  
     		if(read(kfd, &c, 1) < 0){
       		perror("read():");
       		exit(-1);
     		}
 
     		ROS_DEBUG("value: 0x%02X\n", c);
   
   		// read the key and call the convenient server
     		switch(c){
     		
       		case KEYCODE_L:	// Reset positions
        			ROS_DEBUG("LEFT");
        			client_reset_pos.waitForExistence();
        			client_reset_pos.call(r);
        			break;

       		case KEYCODE_U:	// Increase velocity
         			ROS_DEBUG("UP");
				o.request.option = 1;
				client_vel.waitForExistence();
				client_vel.call(o);  
         			break;
         			
       		case KEYCODE_D:	// Decrease velocity
         			ROS_DEBUG("DOWN");
				o.request.option = 2;
				client_vel.waitForExistence();
				client_vel.call(o);  
         			break;
     		}
  	}
  	
	return;
}



int main(int argc, char **argv)
{
	// Main code
	
	// Initialize ros node and its node handle
	ros::init(argc, argv, "user_interface");
	ros::NodeHandle nh;
	
	signal(SIGINT,quit);
	
	// Client of /inc_dec
	client_vel = nh.serviceClient<assignment_2::Inc_dec>("/inc_dec");
	
	// Client of /reset_positions
	client_reset_pos = nh.serviceClient<std_srvs::Empty>("/reset_positions");
 
   	keypressed();
   
   	return 0;

}





