#include "ros_viz_3d.h"
#include <ros/package.h>

double distance(double a_x,double a_y,double b_x,double b_y)
{
       return sqrt((a_x-b_x)*(a_x-b_x)+(a_y-b_y)*(a_y-b_y));
}

ros_viz_3d::ros_viz_3d()
{
	pub = node.advertise<visualization_msgs::MarkerArray>("/TA_markers", 0, this);
  
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

	int ma_id=0;
	bool init;
	
	file.open(file_name.c_str());
	while(file.is_open())
	{
	      init=false;
	      
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
		      marker.type=visualization_msgs::Marker::MESH_RESOURCE;
		      if (topic_name=="/marker_AGENTE2")
		      {
			  marker.mesh_resource = "package://ros_viz_3d/Repair_Kit/Repair_Kit.dae"; // R2D2/R2D2.dae
			  marker.scale.x = 0.3;
			  marker.scale.y = 0.3;
			  marker.scale.z = 0.3;
		      }
	              else
		      { 
			  marker.mesh_resource = "package://ros_viz_3d/forklift/Forklift.dae";
			  marker.scale.x = 0.1;
			  marker.scale.y = 0.1;
			  marker.scale.z = 0.1;
		      }
		      
		      marker.mesh_resource = "package://ros_viz_3d/glider/glider.dae"; //HACK	
			
		      marker.mesh_use_embedded_materials=true;
		      //marker.type=visualization_msgs::Marker::SPHERE;
		      marker.scale.x = 0.1;
		      marker.scale.y = 0.1;
		      marker.scale.z = 0.1;
		      marker.pose.position.z=0;	
		
		      double y;
		      double charge;
		      tf::Quaternion q;
		      std::stringstream   linestream(line);
		      std::string         data;

		      std::getline(linestream, data, ' ');
		      linestream >> marker.pose.position.x >>  marker.pose.position.y >> y >> charge;
		      		      
		      if (topic_name=="/marker_AGENTE2") q.setRPY(0,0,y+M_PI/2); //R2D2
		      else q.setRPY(0,0,y+M_PI/2); //FORKLIFT
		      
		      q.setRPY(0,0,y+M_PI/2); //FORKLIFT HACK
		      
		      marker.pose.orientation.w=q.getW();
		      marker.pose.orientation.x=q.getX();
		      marker.pose.orientation.y=q.getY();
		      marker.pose.orientation.z=q.getZ();
		      
		      marker.lifetime=ros::Duration(1);
		      
		      agents_marker[topic_name].push_back(marker);
		      
		      //CHARGE
		      
		      visualization_msgs::Marker charge_marker;
    
		      charge_marker.header.frame_id = "/base_link";
		      charge_marker.type=visualization_msgs::Marker::TEXT_VIEW_FACING;
		      std::ostringstream charge_strs;
		      charge_strs<< (int)charge;
		      std::string temp_ch = charge_strs.str();
		      temp_ch.append(" %");
		      charge_marker.text=temp_ch;
		      charge_marker.color.a=1;
		      
		      charge_marker.color.r=0;
		      charge_marker.color.g=1;
		      charge_marker.color.b=0;
		      
		      if(charge < 50)
		      {
			    charge_marker.color.r=0.7;
			    charge_marker.color.g=0.7;
			    charge_marker.color.b=0;
		      }
		      
		      if(charge < 30)
		      {
			    charge_marker.color.r=1;
			    charge_marker.color.g=0;
			    charge_marker.color.b=0;
		      }
		      
		      charge_marker.scale.x = 1;
		      charge_marker.scale.y = 1;
		      charge_marker.scale.z = 0.1;
		      charge_marker.pose.position.x=marker.pose.position.x;
		      charge_marker.pose.position.y=marker.pose.position.y;
		      charge_marker.pose.position.z=marker.pose.position.z+0.6;	
		
		      q.setRPY(0,0,0);
		      charge_marker.pose.orientation.w=q.getW();
		      charge_marker.pose.orientation.x=q.getX();
		      charge_marker.pose.orientation.y=q.getY();
		      charge_marker.pose.orientation.z=q.getZ();
		      
		      charge_marker.lifetime=ros::Duration(1);
		      
		      agents_charge[topic_name].push_back(charge_marker);
		      
		      //HOME
		      
		      if(!init)
		      {
			      visualization_msgs::Marker home_marker;
			      
			      home_marker.header.frame_id = "/base_link";
			      home_marker.type=visualization_msgs::Marker::CYLINDER;
			      home_marker.scale.x = 1;
			      home_marker.scale.y = 1;
			      home_marker.scale.z = 0.01;
			      home_marker.pose.position.x=marker.pose.position.x;
			      home_marker.pose.position.y=marker.pose.position.y;
			      home_marker.pose.position.z=marker.pose.position.z;
			
			      q.setRPY(0,0,0);
			      home_marker.pose.orientation.w=q.getW();
			      home_marker.pose.orientation.x=q.getX();
			      home_marker.pose.orientation.y=q.getY();
			      home_marker.pose.orientation.z=q.getZ();
			      
			      home_marker.color.r = 0.662745; 
			      home_marker.color.g = 0.662745;
			      home_marker.color.b = 0.662745;
			      home_marker.color.a=1;
			      
			      home_marker.lifetime=ros::Duration(1);
			      			      
			      agents_home[topic_name]=home_marker;
			      
			      init=true;
		      }
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
	
	ma_id=2;
	long int sim_index;
	int color_switch=0;
	bool active;
	
	file.open(file_name.c_str());
	while(file.is_open())
	{
	      sim_index=0;
	  
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
	      
	      
	      active=true;
	      	      
	      std::string line;
	      while(getline(file,line))
	      {

		      visualization_msgs::Marker marker;
    
		      marker.header.frame_id = "/base_link";

		      marker.pose.position.z=0;
		
		      double execution_time,period,deadline,time;
		      std::string id,owner;
		      int type,executing,available,done;
		      tf::Quaternion q;
		      std::stringstream   linestream(line);
		      std::string         data;

		      std::getline(linestream, data, ' ');

		      linestream >> id >> marker.pose.position.x >>  marker.pose.position.y >> marker.pose.position.z >> type >> execution_time >> period >> deadline >> executing >> time >> owner >> available >> done;

		      q.setRPY(0,0,0);
		      q.setRPY(0,0,M_PI/2); //HACK
		      marker.pose.orientation.w=q.getW();
		      marker.pose.orientation.x=q.getX();
		      marker.pose.orientation.y=q.getY();
		      marker.pose.orientation.z=q.getZ();
		      
		      marker.lifetime=ros::Duration(1);
		      
		      if(active)
		      {
			      marker.color.a=1;
			      
			      if(type==1 || type==2)
			      {
				    marker.type=visualization_msgs::Marker::MESH_RESOURCE;
// 				    marker.mesh_resource = "package://ros_viz_3d/oildrum/oildrum.dae";
				     marker.mesh_resource = "package://ros_viz_3d/glider/glider.dae"; //HACK
				    //marker.mesh_use_embedded_materials=true;
				    //marker.type=visualization_msgs::Marker::CUBE;
				    
				    marker.scale.x = 0.1; //0.25
				    marker.scale.y = 0.1; //0.25
				    marker.scale.z = 0.1; //0.25
				    
// 				    if(type==1)
// 				    {
// 					  marker.color.r = 0.5;
// 					  marker.color.g = 0.5;
// 					  marker.color.b = 0;
// 				    }
// 				    else
// 				    {
// 					  marker.color.r = 0.5;
// 					  marker.color.g = 0;
// 					  marker.color.b = 0.5;
// 				    }

				    marker.color.r = 1;
				    marker.color.g = 0.7;
				    marker.color.b = 0.4;
				    marker.color.a=0.5; //1
				    
				    if(!available && owner!="0")
				    {
					  std::string temp_str="/marker_";
					  temp_str.append(owner);

					  tf::Quaternion q2;
					  q2.setX(agents_marker.at(temp_str).at(sim_index).pose.orientation.x);
					  q2.setY(agents_marker.at(temp_str).at(sim_index).pose.orientation.y);
					  q2.setZ(agents_marker.at(temp_str).at(sim_index).pose.orientation.z);
					  q2.setW(agents_marker.at(temp_str).at(sim_index).pose.orientation.w);
					  double roll_,pitch_,yaw_;
					  tf::Matrix3x3(q2).getRPY(roll_,pitch_,yaw_);
					  
					  //if (owner!="AGENTE2")
					  //{
						  if(type==1)
						  {
							marker.pose.position.x = agents_marker.at(temp_str).at(sim_index).pose.position.x + 0.4*cos(yaw_-M_PI/2);
							marker.pose.position.y = agents_marker.at(temp_str).at(sim_index).pose.position.y + 0.4*sin(yaw_-M_PI/2);
						  }
						  else
						  {
						        marker.pose.position.x = agents_marker.at(temp_str).at(sim_index).pose.position.x + 0.6*cos(yaw_-M_PI/2);
							marker.pose.position.y = agents_marker.at(temp_str).at(sim_index).pose.position.y + 0.6*sin(yaw_-M_PI/2);
						  }  
						    
					  //}
					  //else 
					  //{
					//	  marker.pose.position.x = agents_marker.at(temp_str).at(sim_index).pose.position.x;
					//          marker.pose.position.y = agents_marker.at(temp_str).at(sim_index).pose.position.y;
					 // }
					  
					  marker.pose.position.z = 0.1;
					  
					  if(distance(marker.pose.position.x,marker.pose.position.y,agents_home.at(temp_str).pose.position.x,agents_home.at(temp_str).pose.position.y) < 0.2) active=false;
				    }
			      }
		      

			      if(type==0)
			      {
				    marker.type=visualization_msgs::Marker::CUBE;
				    
				    marker.scale.x = 1;
				    marker.scale.y = 1;
				    marker.scale.z = 0.01;
				    
				    marker.color.r = 0;
				    marker.color.g = 0.2;
				    marker.color.b = 0;
				    marker.color.a=1;
				    
				    if(owner!="0" && !available)
				    {
					  if(color_switch<30)
					  {
					      marker.color.r = 0;
					      marker.color.g = 0.2;
					      marker.color.b = 0;
					      marker.color.a=1;
					      color_switch++;
					  }
					  else
					  {
					      marker.color.r = 0;
					      marker.color.g = 0.4;
					      marker.color.b = 0;
					      marker.color.a=1;
					      color_switch++;
					      if(color_switch==60) color_switch=0;
					  }
				    }
			      }
		      }
		      else
		      {
			      marker.color.a=0;
			      if(available)
			      {
				      active=true;
			      }
		      }
		      
		      tasks_marker[topic_name].push_back(marker);
		      
		      sim_index++;
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
	
	unsigned int i,j,k,l,m,g;
	
	j=0;g=0;

	while(j<agents_marker.at(agents.at(0)).size() && g<tasks_marker.at(tasks.at(0)).size())
	{
		visualization_msgs::MarkerArray ma;
	  
	        for(i=0;i<agents.size();i++)
		{
		         agents_marker.at(agents.at(i)).at(j).id=i;
			 ma.markers.push_back(agents_marker.at(agents.at(i)).at(j));

		}
		
// 		for(l=0;l<agents.size();l++)
// 		{
// 		         agents_home.at(agents.at(l)).id=i+l;
// 			 ma.markers.push_back(agents_home.at(agents.at(l)));
// 		}
		
		for(m=0;m<agents.size();m++)
		{
		         agents_charge.at(agents.at(m)).at(j).id=i+m+l;
			 ma.markers.push_back(agents_charge.at(agents.at(m)).at(j));
		}

		for(k=0;k<tasks.size();k++)
		{
			 tasks_marker.at(tasks.at(k)).at(g).id=i+m+l+k;
			 ma.markers.push_back(tasks_marker.at(tasks.at(k)).at(g));	
		}

		marker_array.push_back(ma);
		j++;g++;
	}
	
	for(i=0;i<marker_array.size();i++)
	{
		pub.publish(marker_array.at(i));
		usleep(10000);
	}
	
	std::cout<<std::endl<<"Markers Finished"<<std::endl;
	
}


ros_viz_3d::~ros_viz_3d()
{

}

