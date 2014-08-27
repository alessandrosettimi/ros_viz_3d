#include "ros_viz_3d.h"

int main(int argc, char *argv[])
{
    if( !ros::isInitialized() )
    {
        ros::init( argc, argv, "ros_viz_3d");
    }

    ros_viz_3d viz;
    
    viz.read();
	
    ros::spin();
    
    return 0;
}