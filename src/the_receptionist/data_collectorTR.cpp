#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Pose2D.h"
#include <math.h>
#include <string>
#include <iostream>

int fr =  10;
int current_person;
int people_counter = 0;
//bool act;

std_msgs::String nSensorsData;
std_msgs::String oSensorsData;
std_msgs::String aSensorsData;
std_msgs::String msg;
std_msgs::Int32 pp;
std_msgs::Int32 pp2;
std_msgs::Bool reset;
std_msgs::Bool done;
std_msgs::Bool old_person;
ros::Publisher talkPub;
ros::Publisher treePub;
ros::Publisher dumpPub;
ros::Publisher observerPub;
ros::Publisher dataPub;
ros::Publisher donePub;
ros::Publisher agePub;


struct Person {

    std::string drink;
    std::string name;
    bool old;
    
};

Person person;

void nameReceived(const std_msgs::String::ConstPtr& name_)
{
	nSensorsData = *name_;
    person.name = nSensorsData.data;
}

void drinkReceived(const std_msgs::String::ConstPtr& drink_)
{
	oSensorsData = *drink_;
    person.drink = oSensorsData.data;
}

void ageReceived(const std_msgs::String::ConstPtr& age_)
{
	aSensorsData = *age_;
    if (aSensorsData.data == "Yes"){
        person.old = true;
    }
    else{
        person.old = false;
    }
    old_person.data = person.old;
    agePub.publish(old_person);
    people_counter++;
}

void dump(ros::Publisher talkPub, ros::Publisher dumpPub, Person person, int people_counter)
{

    std_msgs::String msg;
    std::stringstream ss;
    std::stringstream status;
        
    ss << person.name << "s favorite drink is " << person.drink << ".\n";

    msg.data = ss.str();
	talkPub.publish(msg);

    if(people_counter == 2){

	status << "SUCCESS";
	msg.data = status.str();
	dumpPub.publish(msg);

    done.data = true;
    donePub.publish(done);

    }
    else{
        status << "FAILURE";
	    msg.data = status.str();
	    dumpPub.publish(msg);
    }

}

void dumpTree(const std_msgs::Int32::ConstPtr& pp2){
    dump(talkPub, dumpPub, person, people_counter);
}

void activacionTree(const std_msgs::Int32::ConstPtr& pp)
{ 
    current_person = pp->data;

    if(people_counter != current_person){
        std::stringstream ss;
	    ss << "RUNNING";
	    msg.data = ss.str();
	    treePub.publish(msg);
    }
    
    else{

        std::stringstream ss;
	    ss << "SUCCESS";
	    msg.data = ss.str();
	    treePub.publish(msg);

        reset.data = false;
        observerPub.publish(reset);
        dataPub.publish(reset);
    }
}











int main(int argc, char** argv)
{

	ros::init(argc, argv, "data_collector");
	ros::NodeHandle nh;
	ros::Rate loop_rate(fr);

    //ros::Publisher cPub = nh.advertise<geometry_msgs::Pose2D> ("/controller_instructions", fr, true);
	talkPub = nh.advertise<std_msgs::String>("/msg_to_say", fr);
	treePub = nh.advertise<std_msgs::String>("/status_data", fr);
    dumpPub = nh.advertise<std_msgs::String>("/status_dump", fr);
    observerPub = nh.advertise<std_msgs::Bool>("/reset_observador", fr);
    donePub = nh.advertise<std_msgs::Bool>("/reset_dump", fr);
    dataPub = nh.advertise<std_msgs::Bool>("/reset_data", fr);
    agePub = nh.advertise<std_msgs::Bool>("/old_data", fr);

	ros::Subscriber nameSub = nh.subscribe<std_msgs::String>("/name_data", fr, nameReceived);
    ros::Subscriber drinkSub = nh.subscribe<std_msgs::String>("/drink_data", fr, drinkReceived);
	ros::Subscriber listenerSub = nh.subscribe<std_msgs::String>("/age_data", fr, ageReceived);
	ros::Subscriber Activador = nh.subscribe("/control_data", fr, activacionTree);
    ros::Subscriber Dumper = nh.subscribe("/data_dump", fr, dumpTree);

    ros::spin();
}