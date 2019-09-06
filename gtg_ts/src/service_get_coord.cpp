#include <iostream>
#include "ros/ros.h"
#include "gtg_ts/getSetPosKeyboard.h"
using namespace std;

// FUNCTION SERVICE
bool getFromKB(gtg_ts::getSetPosKeyboard::Request &req, gtg_ts::getSetPosKeyboard::Response &res){
    while(1){
        ROS_INFO("Insert set points WITH Boundaries : 0 <= x,y <= 11.5");
        cout << "Put x-axis: ";
        cin >> res.x;
        cout << "Put y-axis: ";
        cin >> res.y;
        if(res.x < 0 || res.x > 11.5 || res.y < 0 || res.y > 11.5){
            ROS_INFO("ERROR - WRONG SET POINT");
        }
        else{
            break;
        }
    }
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv,"srv_sp_node");
    ros::NodeHandle n;
    ros::ServiceServer srv = n.advertiseService("getSetPosKeyboard", getFromKB);
    ros::spin();
    return 0;
} 
