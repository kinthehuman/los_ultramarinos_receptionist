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
#include <memory>

#include "ros/ros.h"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "behaviortree_cpp_v3/utils/shared_library.h"
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"

#include "ros/package.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "behavior_tree");
  ros::NodeHandle n;

  BT::BehaviorTreeFactory factory;
  BT::SharedLibrary loader;

  factory.registerFromPlugin(loader.getOSName("ObservarTR"));
  factory.registerFromPlugin(loader.getOSName("DataCheckTR"));
  factory.registerFromPlugin(loader.getOSName("DataDumpTR"));
  factory.registerFromPlugin(loader.getOSName("Navegar3TR"));
  factory.registerFromPlugin(loader.getOSName("Navegar2TR"));
  factory.registerFromPlugin(loader.getOSName("ControlDataTR"));
  factory.registerFromPlugin(loader.getOSName("ControlDumpTR"));
  factory.registerFromPlugin(loader.getOSName("ControlObservadorTR"));

  auto blackboard = BT::Blackboard::create();

  std::string pkgpath = ros::package::getPath("The-Receptionist");
  std::string xml_file = pkgpath + "/behavior_trees_xml/the_receptionist.xml";

  BT::Tree tree = factory.createTreeFromFile(xml_file);
  auto publisher_zmq = std::make_shared<BT::PublisherZMQ>(tree, 10, 1666, 1667);

  ros::Rate loop_rate(10);

  bool finish = false;
  while (ros::ok() && !finish)
  {
    finish = tree.rootNode()->executeTick() == BT::NodeStatus::SUCCESS;

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
