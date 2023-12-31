// Copyright 2022 los ultramarinos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

#include "behavior_tree/Navegar3TR.h"


namespace behavior_trees
{


Navegar3TR::Navegar3TR(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack("")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  //angle = nh_.advertise<geometry_msgs::PoseStamped>("/angle_calc", 10);
  sub = nh_.subscribe("/move_base/result", 10, &Navegar3TR::messageCallback, this);
  ageSub = nh_.subscribe("/old_data", 10, &Navegar3TR::ageCallback, this);
  resetSub = nh_.subscribe("/navegar_reset", 10, &Navegar3TR::resetCallback, this);
  talkPub = nh_.advertise<std_msgs::String>("/msg_to_say", 10);


    sofa.header.stamp = i;
    sofa.header.frame_id = "map";

    sofa.pose.position.x = 6.8;
    sofa.pose.position.y = 3.75;
    sofa.pose.position.z = 0.0;

    sofa.pose.orientation.x = 0.0;
    sofa.pose.orientation.y = 0.0;
    sofa.pose.orientation.z = 0.0;
    sofa.pose.orientation.w = 1.0;


    chairs.header.stamp = i;
    chairs.header.frame_id = "map";

    chairs.pose.position.x = 1.3;
    chairs.pose.position.y = 5;
    chairs.pose.position.z = 0.0;

    chairs.pose.orientation.x = 0.0;
    chairs.pose.orientation.y = 0.0;
    chairs.pose.orientation.z = 0.0;
    chairs.pose.orientation.w = 1.0;

}

void Navegar3TR::ageCallback(const std_msgs::Bool::ConstPtr& age){
  old = age->data;
}
void Navegar3TR::resetCallback(const std_msgs::Bool::ConstPtr& reset){
  counter = 0;
}

void Navegar3TR::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}

void Navegar3TR::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus Navegar3TR::tick()
{
  if (a == 5)
  {
    std::cout << a << "\n";
    

    if(old){
      result = sofa;
    }
    else{
      result = chairs;
    }
    say.data = "Follow me please";
    talkPub.publish(say);
    activador.publish(result);
    
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {

      //angle.publish(result);
      a = 0;

      feedBack = "";
      return BT::NodeStatus::SUCCESS;
    }
    else
    {
      return BT::NodeStatus::SUCCESS;
    }
  }
  return BT::NodeStatus::RUNNING;
}
}  // namespace behavior_trees

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Navegar3TR>("Navegar3TR");
}
