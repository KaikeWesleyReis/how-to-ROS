# ROS - Go to Goal with Turtlesim
## Requirements
- **Version**: Kinetic 1.12.14
- **OS**: Ubuntu 16.04

## Overview
Written in **Python** and **C++**, this ROS package introduce a different way to understand a service/client and publisher/subscriber application in ROS from the basic tutorials that you can find here: http://wiki.ros.org/ROS/Tutorials. Given the major differences between C++ and python for ROS, this package was written in both languages. This allows the user to better understand major differences in creating ROS codes using one or another.

## Workflow
A *Service node* will be responsable to get SET POINT COORDINATE (your goal) in (x,y) and a *Publisher/Subscriber/Client node* will order the turtle to destination through angular velocity using Manhattan Distance concept.

**Example**: The turtlesim usually start your turtle in (5.5,5.5). The gif bellow shows this package working for a set point equals to **(2,3)**.

![ts](https://user-images.githubusercontent.com/32513366/57415098-3999e000-71d1-11e9-9e18-c5f41697a4d8.gif)
   
**Infos**:  Some parameters was essentials to create the commander node as the accepted errors (stop conditions) and they are
- Accepted error for linear move: 0.05
- Accepted error for angular rotation: 0.001
   
**PS**: Using Python or C++ inevitably you will need to change something in *CMakeLists.txt* or *package.xml*. This is cover in ROS tutorials, but any questions that you have you can send here (or do reverse engineering in my files).
   
## HOW TO ...
### INSTALL THIS PACKAGE
In your **src** folder from your catkin worspace, open a terminal:
**FAZENDODDOSDAOASDO**

        > roscore
        
        > rosrun turtlesim turtlesim_node
        

### RUN IN C++
**(!)** For each line open a new terminal in your system

        > roscore
        
        > rosrun turtlesim turtlesim_node
        
        > rosrun gtg_ts service_get_coord
        
        > rosrun gtg_ts cliente_move
        
        > rostopic echo /turtle1/cmd_vel #OPTIONAL - To see turtle speed in real time
        
        > rostopic echo /turtle1/pose #OPTIONAL - To see turtle postion in real time


### RUN IN PYTHON
**(!)** For each line open a new terminal in your system

        > roscore
        
        > rosrun turtlesim turtlesim_node
        
        > rosrun gtg_ts gotogoal_manhattan.py
        
        > rosrun gtg_ts keyboard_sp_node.py
        
        > rostopic echo /turtle1/cmd_vel #OPTIONAL - To see turtle speed in real time
        
        > rostopic echo /turtle1/pose #OPTIONAL - To see turtle postion in real time
