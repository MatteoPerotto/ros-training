#include "ros/ros.h"
#include "assignment1/RandomPosition.h"
#include "robot_description/Position.h"

int main(int argc, char** argv)
{	
	ros::init(argc,argv, "control_caller_node");
	ros::NodeHandle  n;
	
	//---- Services ----//

	// Client 1 - call the random position server
	ros::ServiceClient client1 = n.serviceClient<assignment1::RandomPosition>("/generate_random_position");
	assignment1::RandomPosition service1;
	service1.request.x_min = 0.0;
	service1.request.x_max = 1.0;
	service1.request.y_min = 0.0;
	service1.request.y_max = 9.0;
	client1.waitForExistence();
	
	// Client 2 - call the gtp server  
	ros::ServiceClient client2 = n.serviceClient<robot_description::Position>("/go_to_point");
	robot_description::Position service2;
	client2.waitForExistence(); 
	
	while(ros::ok())
	{
		client1.call(service1); 
		float xpos = service1.response.x_rand;
		float ypos = service1.response.y_rand;
		float theta = service1.response.theta_rand;
		
		service2.request.x = xpos;
		service2.request.y = ypos;
		service2.request.theta = theta;
		client2.call(service2);
	}

	return 0;
}
