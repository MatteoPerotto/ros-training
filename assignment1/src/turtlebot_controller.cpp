#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/TwistWithCovariance.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/Kill.h"

// Global 
float vx;
float vy;
float theta_dot;

void odometryCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
	vx = odom->twist.twist.linear.x; 
	vy = odom->twist.twist.linear.y; 
	theta_dot = odom->twist.twist.angular.z; 
}	

int main(int argc, char** argv)
{	
	
	ros::init(argc,argv, "turtlebot_controller");
	ros::NodeHandle  n;
		
	//----- Publishers/Subscribers ----//
	
	// Subscriber 1 - subscribe to the odometry of the robot 
	ros::Subscriber sub1 = n.subscribe<nav_msgs::Odometry>("odom",1,odometryCallback);
	
	
	// Publisher 1 - publish odometry to the turtle
	ros::Publisher pub1 = n.advertise<geometry_msgs::Twist>("rpr_turtle/cmd_vel",1);
	
	
	//---- Services ----//
	
	// Client 1 - spawn a new turtle in robot position
	ros::ServiceClient client1 = n.serviceClient<turtlesim::Spawn>("/spawn");
	turtlesim::Spawn service1;
	service1.request.x = 5.5;
	service1.request.y = 5.5;
	service1.request.theta = 0.0;
	service1.request.name = "rpr_turtle";
	client1.waitForExistence();
	client1.call(service1);
		
	// Client 2 - kill the old turtle 	
	ros::ServiceClient client2 = n.serviceClient<turtlesim::Kill>("/kill");
	turtlesim::Kill service2;
	service2.request.name = "turtle1";
	client2.waitForExistence(); 
	client2.call(service2);
	
	
	while(ros::ok())
	{	
		geometry_msgs::Twist vel;
				
		vel.linear.x = vx;
		vel.linear.y = vy;
		vel.angular.z = theta_dot;
	
		pub1.publish(vel);
		ros::spinOnce();
	}
		
	return 0; 	
}
