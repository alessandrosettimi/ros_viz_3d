#include "ros_viz_3d.h"
#include <ros/package.h>

ros_viz_3d::ros_viz_3d()
{
	std::string path_to_package = ros::package::getPath("ros_viz_3d");
	std::string path_to_agent_files = path_to_package; path_to_agent_files.append("/files/ta_log_AGENTE");
	std::string path_to_task_files = path_to_package; path_to_task_files.append("/files/ta_log_T");
    
        agent_base_string=path_to_agent_files;
	task_base_string=path_to_task_files;
	
	unsigned int i=1;
	
	std::string file_name=agent_base_string;
	std::stringstream iter;
	iter.str("");
	iter << i;
	file_name.append(iter.str());
	file_name.append(".txt");
	
	std::cout<<"AGENTS:"<<std::endl;

	file.open(file_name.c_str());
	while(file.is_open())
	{
	      std::string topic_name="/marker_";
	      std::string temp="AGENTE";
	      iter.str("");
	      iter << i;
	      temp.append(iter.str());
	      topic_name.append(temp);

	      agents_pubs[topic_name] = node.advertise<visualization_msgs::Marker>(topic_name, 0, this);
	      agents_files[temp] = file_name;
	      agents.push_back(topic_name);
	      
	      std::cout<<" - "<<topic_name<<std::endl;
	      std::cout<<" - "<<file_name<<std::endl;
	      
	      std::cout<<" --- creating markers"<<std::endl;
	      
	      std::string line;
	      while(getline(file,line))
	      {
		      visualization_msgs::Marker marker;
    
		      marker.header.frame_id = "/base_link";
		      //marker.type=visualization_msgs::Marker::MESH_RESOURCE;
		      //marker.mesh_resource = "package://pilot_interface/src/main_interface/objects/valve/vrc_valve/meshes/vrc_valve.dae";
		      marker.type=visualization_msgs::Marker::SPHERE;
		      marker.scale.x = 1;
		      marker.scale.y = 1;
		      marker.scale.z = 1;
		      marker.color.r = 1;
		      marker.color.g = 0;
		      marker.color.b = 0;
		      marker.color.a = 1;
		      marker.pose.position.z=0;	
		
		      double y;
		      tf::Quaternion q;
		      std::stringstream   linestream(line);
		      std::string         data;

		      std::getline(linestream, data, ' ');
		      linestream >> marker.pose.position.x >>  marker.pose.position.y >> y;
		      
		      q.setRPY(0,0,y);
		      marker.pose.orientation.w=q.getW();
		      marker.pose.orientation.x=q.getX();
		      marker.pose.orientation.y=q.getY();
		      marker.pose.orientation.z=q.getZ();
		      
		      agents_marker[topic_name].push_back(marker);
	      }
	      
	      file.close();
	      
	      i++;
	      file_name=agent_base_string;
	      iter.str("");
	      iter << i;
	      file_name.append(iter.str());
	      file_name.append(".txt");
	      file.open(file_name.c_str());
	}
	
	i=1;
	
	file_name=task_base_string;
	iter.str("");
	iter << i;
	file_name.append(iter.str());
	file_name.append(".txt");
	
	std::cout<<"TASKS:"<<std::endl;
	
	file.open(file_name.c_str());
	while(file.is_open())
	{
	      std::string topic_name="/marker_";
	      std::string temp="T";
	      iter.str("");
	      iter << i;
	      temp.append(iter.str());
	      topic_name.append(temp);

	      tasks_pubs[topic_name] = node.advertise<visualization_msgs::Marker>(topic_name, 0, this);
	      tasks_files[temp] = file_name;
	      tasks.push_back(topic_name);
	      
	      std::cout<<" - "<<topic_name<<std::endl;
	      std::cout<<" - "<<file_name<<std::endl;
	      
	      
	      std::cout<<" --- creating markers"<<std::endl;
	      
	      std::string line;
	      while(getline(file,line))
	      {
		      visualization_msgs::Marker marker;
    
		      marker.header.frame_id = "/base_link";
		      //marker.type=visualization_msgs::Marker::MESH_RESOURCE;
		      //marker.mesh_resource = "package://pilot_interface/src/main_interface/objects/valve/vrc_valve/meshes/vrc_valve.dae";
		      marker.type=visualization_msgs::Marker::CUBE;
		      marker.scale.x = 1;
		      marker.scale.y = 1;
		      marker.scale.z = 1;
		      marker.color.r = 1;
		      marker.color.g = 0;
		      marker.color.b = 0;
		      marker.color.a = 1;
		      marker.pose.position.z=0;	
		
		      double y;
		      tf::Quaternion q;
		      std::stringstream   linestream(line);
		      std::string         data;

		      std::getline(linestream, data, ' ');
		      linestream >> marker.pose.position.x >>  marker.pose.position.y >> y;
		      
		      q.setRPY(0,0,y);
		      marker.pose.orientation.w=q.getW();
		      marker.pose.orientation.x=q.getX();
		      marker.pose.orientation.y=q.getY();
		      marker.pose.orientation.z=q.getZ();
		      
		      tasks_marker[topic_name].push_back(marker);
	      }
	      
	      
	      file.close();
	      
	      i++;
	      file_name=task_base_string;
	      iter.str("");
	      iter << i;
	      file_name.append(iter.str());
	      file_name.append(".txt");
	      file.open(file_name.c_str());
	}
}

void ros_viz_3d::read()
{
	std::cout<<std::endl<<"Publishing Markers"<<std::endl;
	
	unsigned int i,j,k,g;
	
	j=0;g=0;

	while(j<agents_marker.at(agents.at(0)).size() && g<tasks_marker.at(tasks.at(0)).size())
	{

	        for(i=0;i<agents.size();i++)
		{
			 agents_pubs.at(agents.at(i)).publish(agents_marker.at(agents.at(i)).at(j));
		}

		for(k=0;k<tasks.size();k++)
		{
		         tasks_pubs.at(tasks.at(k)).publish(tasks_marker.at(tasks.at(k)).at(g));
		}

		j++;g++;
	}
	
	std::cout<<std::endl<<"Markers Finished"<<std::endl;
	
}


ros_viz_3d::~ros_viz_3d()
{

}

