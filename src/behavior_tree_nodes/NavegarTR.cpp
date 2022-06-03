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

#include "behavior_tree/NavegarTR.h"

namespace behavior_trees
{
NavegarTR::NavegarTR(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack("")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  //poseSub = nh_.subscribe("/navegar_arbitro", 10, &NavegarTR::posicionNav, this);
  sub = nh_.subscribe("/move_base/result", 10, &NavegarTR::messageCallback, this);
}

void NavegarTR::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}
/*
void NavegarTR::posicionNav(const geometry_msgs::PoseStamped::ConstPtr& pose){
  result = *pose;
}
*/

void NavegarTR::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus NavegarTR::tick()
{
  if (a == 5)
  {
    std::cout << a << "\n";

    result.header.stamp = i;
    result.header.frame_id = "map";

    result.pose.position.x = 0.0;
    result.pose.position.y = 0.0;
    result.pose.position.z = 0.0;

    result.pose.orientation.x = 0.0;
    result.pose.orientation.y = 0.0;
    result.pose.orientation.z = 0.0;
    result.pose.orientation.w = 1.0;
    
    activador.publish(result);
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
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
  factory.registerNodeType<behavior_trees::NavegarTR>("NavegarTR");
}
