#include "ros/ros.h"
#include "assignment1/RandomPosition.h"
#include <math.h>

double randMToN( double M, double N)
{return M + (rand() / ( RAND_MAX / (N-M) ));}

bool myrandom(assignment1::RandomPosition::Request &req, assignment1::RandomPosition::Response &res)
{
	res.x_rand = randMToN(req.x_min, req.x_max);
	res.y_rand = randMToN(req.y_min, req.y_max);
	res.theta_rand = randMToN(0, 2*M_PI);
	return true;
}

int main(int argc, char** argv)
{	
	
	ros::init(argc,argv, "random_position_server");
	ros::NodeHandle  n;
	
	ros::ServiceServer service = n.advertiseService("/generate_random_position",myrandom);
	ros::spin();

	return 0; 	
}



