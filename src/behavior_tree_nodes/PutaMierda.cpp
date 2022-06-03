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

#include "behavior_tree/PutaMierda.h"

namespace behavior_trees
{

PutaMierda::PutaMierda(const std::string& name , const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_()
{
  	talkPub = nh_.advertise<std_msgs::String>("/msg_to_say", 10);
}


void PutaMierda::halt()
{
  //ROS_INFO("Seguir halt");
  //std_msgs::Bool act;
  //act.data = false;
  //activador.publish(act);
}

BT::NodeStatus PutaMierda::tick()
{
  if (a == 0){
    std::cout << "LISTENING...\n";
  }

  a++;

  if(a == 57){
    done == true;
  }

  if (done)
  {
    msg.data = "ACTIVATING ORDER 66";
    talkPub.publish(msg);
    std::cout << "LESGOOOOO\n";
    return BT::NodeStatus::SUCCESS;
  }
  else{
     return BT::NodeStatus::RUNNING;
  }
}
}  // namespace behavior_trees


BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::PutaMierda>("PutaMierda");
}