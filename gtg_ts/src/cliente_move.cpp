#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cmath>
#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "gtg_ts/getSetPosKeyboard.h"
#define PI 3.14159265358979
using namespace std;

// AUTHOR - KAIKE WESLEY REIS

class ctrl_turtle{
    public:
    //CLASS CONSTRUCTOR
    ctrl_turtle() : rate(10) 
    {
        //DECLARING CLIENT
        this->clt = n.serviceClient<gtg_ts::getSetPosKeyboard>("getSetPosKeyboard");
        //DECLARING PUBLISHER
        this->pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
        //DECLARING SUBSCRIBER
        this->sub = n.subscribe("/turtle1/pose",100, &ctrl_turtle::cbc_func, this);
        //INITIALIZE VELOCITY
        this->vel_msg.linear.y = 0;
        this->vel_msg.linear.z = 0;
        this->vel_msg.angular.x = 0;
        this->vel_msg.angular.y = 0;
        // HYPER PARAMETERS
        this->vel_max = 2;               //MAXIMUM SPEED FOR OUR TURTLE
        this->psi = 0.01;                //psi% IS THE MINIMUM SPEED FOR OUR TURTLE
        this->error = 0.05;              //IS THE THRESHOLD ACCEPTED ERROR FOR LINEAR MOVE
        this->er_ang = 0.001;            //IS THE THRESHOLD ACCEPTED ERROR FOR ANGULAR MOVE
    }
    //FUNCTION CALL BACK
    void cbc_func(const turtlesim::Pose::ConstPtr& msg)
    {
        this->loc = *msg;
        //ROS_INFO("CB - x %f y %f",this->loc.x, this->loc.y);
    }
    //FUNCTION TO CALL A SERVICE BY CLIENT
    void clt_getSP()
    {
        if(clt.call(srv)){
            this->sp_x = srv.response.x;
            this->sp_y = srv.response.y;
            ROS_INFO("Destination is X %.2f and Y %.2f", this->sp_x,this->sp_y);
        }
        else{
            ROS_ERROR("Not returning srv ...");
        }
    }
    //FUNCTION TO ROTATE TURTLE ORIENTATION
    void rotacione_angulo(float angRadianos)
    {
        ROS_INFO("ROTATE TO ANGLE %f [rad]", angRadianos);
        //SITUATION - ZERO DEGREE TARGET
        if(angRadianos == 0.00)
        {
            this->vel_max = -2;  // MOVE IN CLOCKWISE 
        }
        //DISTANCE IN [rad] FOR OUR TARGET
        float distAngMax = abs(this->loc.theta - angRadianos);
        //LOOP TO ROTATE ORIENTATION TO DESIRED ANGLE
        while(abs(this->loc.theta - angRadianos) > this->er_ang)
        {
            //SPEED DUMPING
            this->dumping = abs(this->loc.theta - angRadianos)/distAngMax;
            //KEEP SPEED AT A MINIMUM THRESHOLD PCT
            if(this->dumping < this->psi){
                this->dumping = this->psi;
            }
            //PUBLISHING SPEED
            this->vel_msg.angular.z = this->vel_max*this->dumping;
            this->pub.publish(this->vel_msg);
            //UPDATE LOCATION BY CALLING OUR CALLBACK FUNCTION ONCE
            ros::spinOnce();
        }
        //FINISH ROTATION
        if(angRadianos == 0.00)
        {
            this->vel_max = 2;
            this->vel_msg.linear.x = 0;
            this->vel_msg.angular.z = 0;
            this->pub.publish(vel_msg);
        }
        else
        {
            this->vel_msg.linear.x = 0;
            this->vel_msg.angular.z = 0;
            this->pub.publish(vel_msg);
        } 
    }
    //LINEAR MOVIMENT IN X
    void movimente_linear_x()
    {
        ROS_INFO("MOVE IN X");
        float MaxDistX = abs(this->loc.x-this->sp_x);
        //LOOP TO GET INTO THE DESIRED SET POINT X
        while((abs(this->loc.x-this->sp_x) > this->error) && !ros::isShuttingDown())
        {
            //SPEED DUMPING
            this->dumping = abs(this->loc.x-this->sp_x)/MaxDistX;
            //KEEP SPEED AT psi% MINIMUM VALUE
            if(this->dumping < this->psi){
                this->dumping = this->psi;
            }
            //PUBLISH SPEED
            this->vel_msg.linear.x = this->vel_max*this->dumping;
            this->pub.publish(this->vel_msg);
            //SPIN TO GET LOCATION BY CALLING CALLBACK FUNCTION
            ros::spinOnce();
        }
        //STOP SPEED
        this->vel_msg.linear.x = 0;
        this->pub.publish(this->vel_msg);
    }
    //LINEAR MOVIMENT IN Y
    void movimente_linear_y()
    {
        ROS_INFO("MOVE IN Y");
        float MaxDistY = abs(this->loc.y-this->sp_y);
        //LOOP TO GET INTO THE DESIRED SET POINT Y
        while((abs(this->loc.y-this->sp_y) > this->error) && !ros::isShuttingDown()){
            //SPEED DUMPING
            this->dumping = abs(this->loc.y-this->sp_y)/MaxDistY;
            //KEEP SPEED AT psi% MINIMUM VALUE
            if(this->dumping < this->psi){
                this->dumping = this->psi;
            }
            //PUBLISH SPEED
            this->vel_msg.linear.x = this->vel_max*this->dumping;
            this->pub.publish(this->vel_msg);
            //SPIN TO GET LOCATION BY CALLING CALLBACK FUNCTION
            ros::spinOnce();
        }
        //STOP SPEED
        this->vel_msg.linear.x = 0;
        this->pub.publish(this->vel_msg);
    }
    //DEFINE QUADRANT
    char sp_quadrantes_loc()
    {
        //SPIN TO GET LOCATION BY CALLING CALLBACK FUNCTION
        ros::spinOnce();
        char r;
        //FIRST QUADRANT
        if((this->sp_y > this->loc.y) && (this->sp_x > this->loc.x))
        {
            r = '1';
            return r;
        }
        //SECOND QUADRANT
        else if((this->sp_y > this->loc.y) && (this->sp_x < this->loc.x))
        {
            r = '2';
            return r;
        }
        //THIRD QUADRANT
        else if((this->sp_y <= this->loc.y) && (this->sp_x <= this->loc.x))
        {
            r = '3';
            return r;
        }
        //FOURTH QUADRANT
        else if((this->sp_y < this->loc.y) && (this->sp_x > this->loc.x))
        {
            r = '4';
            return r;
        }
        //OTHERWISE - THIS CAN BE USE TO SEND A ERROR CHAR
        else
        {
            r = 'w';
            return r;
        }
    }
           
    private:
        //ROS INIT
        ros::NodeHandle n;
        ros::ServiceClient clt;
        ros::Publisher pub;
        ros::Subscriber sub;
        ros::Rate rate;
        gtg_ts::getSetPosKeyboard srv;
        //VAR - VELOCITY MSG
        geometry_msgs::Twist vel_msg;
        //VAR - POSITION
        turtlesim::Pose loc;
        //HYPER PARAMETERS - ACCEPTED ERROS
        float error;  //A.E. for Position
        float er_ang; //A.E. for Orientation
        //HYPER PARAMETERS - VELOCITY
        float vel_max;
        float psi;
        //SET POINT COORDINATES
        float sp_x;
        float sp_y;        
        //OTHER VARs
        float dumping;
        float distAngMax;        
        
};

int main(int argc, char **argv){
    ros::init(argc, argv, "clt_move_node");
    ctrl_turtle ct;
    //DEFINE ANGLES - NAME IN [DEGREES] AND VALUE IN [RAD]
    float a0 = 0.00;
    float a90 = PI/2;
    float a180 = PI;
    float a270 = (3*PI)/2;
    char route;
    //LOOP TO MOVE
    while(!ros::isShuttingDown()){
        //1 - CALL SERVICE TO GET SET POINT
        ct.clt_getSP();
        //2 - DEFINE YOUR ROUTINE BY QUADRANTS
        route = ct.sp_quadrantes_loc();
        ROS_INFO("Route = %c",route);
        //3 - FOLLOW A SPECIFIC ROUTINE BASED IN A QUADRANT
        if(route == '1'){
            //STEP 1 - MOVE IN X
            ct.movimente_linear_x();
            //STEP 2 - ROTATE TO 90 DEGREES
            ct.rotacione_angulo(a90);
            //STEP 3 - MOVE IN Y
            ct.movimente_linear_y();
            //STEP 4 - ROTATE TO ORIGIN
            ct.rotacione_angulo(a0);
        }
        else if(route == '2'){
            //STEP 1 - ROTATE TO 90 DEGREES
            ct.rotacione_angulo(a90);
            //STEP 2 - MOVE IN Y
            ct.movimente_linear_y();
            //STEP 3 - ROTATE TO 180 DEGREES
            ct.rotacione_angulo(a180);
            //STEP 4 - MOVE IN X
            ct.movimente_linear_x();
            //STEP 5 - ROTATE TO ORIGIN
            ct.rotacione_angulo(a0);
        }
        else if(route == '3'){
            //STEP 1 - ROTATE TO 180 DEGREES
            ct.rotacione_angulo(a180);
            //STEP 2 - MOVE IN X
            ct.movimente_linear_x();
            //STEP 3 - ROTATE TO 270 DEGREES
            ct.rotacione_angulo(a270);
            //STEP 4 - MOVE IN Y
            ct.movimente_linear_y();
            //STEP 5 - ROTATE TO ORIGIN
            ct.rotacione_angulo(a0);
        }
        else if(route == '4'){
            //STEP 1 - MOVE IN X
            ct.movimente_linear_x();
            //STEP 2 - ROTATE TO 270 DEGREES
            ct.rotacione_angulo(a270);
            //STEP 3 - MOVE IN Y
            ct.movimente_linear_y();
            //STEP 4 - ROTATE TO ORIGIN
            ct.rotacione_angulo(a0);
        }
    }
}