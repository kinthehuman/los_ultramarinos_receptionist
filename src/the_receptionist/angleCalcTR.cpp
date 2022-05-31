#include <iostream>
#include <cmath>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"


struct Quaternion
{
    double w, x, y, z;
};

struct Vector
{
    double x, y;  
};

Vector arbitro {
    0,
    0,
};

Vector last_position;
ros::Publisher out;
double angle;
Quaternion quat;
geometry_msgs::PoseStamped result;
geometry_msgs::PoseStamped data;
ros::Time i;
int fr = 10;



double yaw (Vector actual , Vector mirar )
{
    // calculo el vector entre Aatual y mirar
    Vector am;
    am.x = mirar.x - actual.x;
    am.y = mirar.y - actual.y;
   
    // vector uni (1,0)
    Vector uni;
    uni.x = 1;
    uni.y = 0;
   
    // angulo en entre uni y am [ ( u . v )/( |u| * |v| )  ]
    double angulo = ( am.x * uni.x + am.y * uni.y ) / sqrt( pow(am.x,2) + pow(am.y,2) ) * sqrt( pow(uni.x,2) + pow(uni.y,2) );
   
    return angulo ;
}  


Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

void angleCalc(const geometry_msgs::PoseStamped::ConstPtr& msg)
{   
    
    data = *msg;
    last_position.x = data.pose.position.x;
    last_position.y = data.pose.position.y;

    angle = yaw(arbitro, last_position);
    quat = ToQuaternion(angle, 0, 0);

    result.header.stamp = i;
    result.header.frame_id = "map";

    result.pose.position.x = arbitro.x;
    result.pose.position.y = arbitro.y;
    result.pose.position.z = 0.0;

    result.pose.orientation.x = quat.x;
    result.pose.orientation.y = quat.y;
    result.pose.orientation.z = quat.z;
    result.pose.orientation.w = quat.w;

    out.publish(result);
    

}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "angleCalc");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);

    out = nh.advertise<geometry_msgs::PoseStamped>("/navegar_arbitro", fr);

    ros::Subscriber in = nh.subscribe("/angle_calc", fr, angleCalc);
    // hola jajajajaj

    ros::spin();
}