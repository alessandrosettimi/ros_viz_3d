#ifndef ROS_VIZ_3D_H
#define ROS_VIZ_3D_H

#include <ros/ros.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <QString>
#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"
#include "tf/transform_datatypes.h"
#include <tf/tf.h>
#include <tf/transform_listener.h>

class ros_viz_3d
{

public:
	ros_viz_3d();
	~ros_viz_3d();
	void read();
private:
	ros::NodeHandle node;
	
	std::vector<std::string> agents, tasks;
	
	std::map<std::string,ros::Publisher> agents_pubs;
	std::map<std::string,ros::Publisher> tasks_pubs;
	
	std::map<std::string,std::string> agents_files;
	std::map<std::string,std::string> tasks_files;
	
	std::map<std::string,std::vector<visualization_msgs::Marker>> agents_marker;
	std::map<std::string,std::vector<visualization_msgs::Marker>> tasks_marker;
	
	std::map<std::string,visualization_msgs::Marker> agents_home;
	std::map<std::string,std::vector<visualization_msgs::Marker>> agents_charge;
		
	std::vector<visualization_msgs::MarkerArray> marker_array;
	ros::Publisher pub;

	std::string agent_base_string;
	std::string task_base_string;
	
	std::ifstream file;
	
};

#endif // ROS_VIZ_3D_H
