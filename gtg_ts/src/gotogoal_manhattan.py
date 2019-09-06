#!/usr/bin/env python
import rospy
from control_turtle_setpos.srv import getSetPosKeyboard
from geometry_msgs.msg import Twist
from std_msgs.msg import String
from turtlesim.msg import Pose
from math import pi as piValue
from math import atan2
from math import sqrt
from math import pow

# AUTHOR - KAIKE WESLEY REIS

class blastoiseSP:
    def __init__(self):
        # 1 - START NODE
        rospy.init_node('control_blastoise')
        # 2 - DEFINE OUR PUBLISHER AND SUBSCRIBER
        self.pubVel = rospy.Publisher('/turtle1/cmd_vel',Twist, queue_size=10)
        self.subPos = rospy.Subscriber('/turtle1/pose', Pose, self.cb_att_loc)
       # 3 - VARIABLES TO HOLD TURTLE POSITION AND KEEPS THE ROUTINE TO MOVE
        self.loc = Pose()
        self.routine = 'w'
        # 4 - VARIABLE TO KEEP AND INITIALIZE TURTLE VELOCITY
        self.vel_msg = Twist()
        self.vel_msg.linear.y = 0
        self.vel_msg.linear.z = 0
        self.vel_msg.angular.x = 0
        self.vel_msg.angular.y = 0
        # 5 - RATE
        self.taxa = rospy.Rate(10)
        # 6 - ACCEPTED ERROR
        self.error = 0.05
        self.errorAng = 0.001
        # 7 - MAX VELOCITY
        self.velMax = 2

    # CALLBACK - READ TURTLE POSITION
    def cb_att_loc(self, data):
        self.loc = data
        self.loc.x = round(self.loc.x, 4)
        self.loc.y = round(self.loc.y, 4)
        self.loc.theta = round(self.loc.theta, 4)
    
    # CLIENT - GET SET POINT X AND Y
    def clt_getSP(self):
        # WAITING
        rospy.wait_for_service('srv_getSP')
        try:
            # GET THE SERVICE
            srv_getSP = rospy.ServiceProxy('srv_getSP',getSetPosKeyboard)
            # GET THE ANSWER
            axes = srv_getSP()
            # SAVING THE SET POINT
            self.sp_x = axes.x
            self.sp_y = axes.y
            print 'GOT SET POINTS: X - ',self.sp_x,' Y - ',self.sp_y                      
        except rospy.ServiceException, e:
            print "Service FAIL: %s"%e

    # FUNCTION - ROTATE ANGLE
    def rotacione_angulo(self, angRadianos):
        # IF WE HAVE TO COME BACK TO ORIGIN ZERO DEGREES
        if angRadianos == 0.00:
            self.velMax = -2
        # GET THE MAXIMUM ANGLE DISTANCE 
        distAngMax = abs(self.loc.theta - angRadianos)
        # LOOP TO ROTATE TO DESIRED ANGLE
        while abs(self.loc.theta - angRadianos) > self.errorAng:
            amortecimento = abs(self.loc.theta - angRadianos)/distAngMax
            # KEEP THE MINIMUM SPEED == 1 PER CENT FROM MAXIMUM SPEED
            if amortecimento < 0.01:
                amortecimento = 0.01
            # PUBLISH BROKEN VELOCITY
            self.vel_msg.angular.z = self.velMax*amortecimento
            self.pubVel.publish(self.vel_msg)
        # STOP ROTATION
        self.vel_msg.angular.z = 0
        self.pubVel.publish(self.vel_msg)     
        # GARANTEE THAT ALL VELOCITIES ARE ZERO AT THE END
        if angRadianos == 0.00:
            self.velMax = 2 
            self.vel_msg.linear.x = 0
            self.vel_msg.angular.z = 0
            self.pubVel.publish(self.vel_msg)

    # FUNCTION - LINEAR MOVIMENT IN X
    def movimente_linear_x(self):
        # GET THE MAXIMUM X DISTANCE 
        MaxDistX = abs(self.loc.x - self.sp_x)
        # LOOP TO GET INTO THE DESIRED SET POINT X COORDINATE
        while abs(self.loc.x - self.sp_x) > self.error:
            amortecimento = abs(self.loc.x - self.sp_x)/MaxDistX
            # KEEP THE MINIMUM SPEED == 1 PER CENT FROM MAXIMUM SPEED
            if amortecimento < 0.01:
                amortecimento = 0.01 
            # PUBLISH BROKEN VELOCITY        
            self.vel_msg.linear.x =  self.velMax*amortecimento
            self.pubVel.publish(self.vel_msg)
        # STOP VELOCITY
        self.vel_msg.linear.x =  0
        self.pubVel.publish(self.vel_msg)

    # FUNCTION - LINEAR MOVIMENT IN Y
    def movimente_linear_y(self):
        # GET THE MAXIMUM X DISTANCE 
        MaxDistY = abs(self.loc.y - self.sp_y)
        # LOOP TO GET INTO THE DESIRED SET POINT Y COORDINATE
        while abs(self.loc.y - self.sp_y) > self.error:
            amortecimento = abs(self.loc.y - self.sp_y)/MaxDistY
            # KEEP THE MINIMUM SPEED == 1 PER CENT FROM MAXIMUM SPEED
            if amortecimento < 0.01:
                amortecimento = 0.01
            # PUBLISH BROKEN VELOCITY 
            self.vel_msg.linear.x =  self.velMax*amortecimento
            self.pubVel.publish(self.vel_msg)
        # STOP VELOCITY
        self.vel_msg.linear.x =  0
        self.pubVel.publish(self.vel_msg)        

    # FUNCTION - DECIDES WHERE IS OUR SET POINT AND THE ROUTINE TO GET THERE
    def sp_quadrantes_loc(self):
        # 1Q
        if (self.sp_y > self.loc.y) and (self.sp_x > self.loc.x):
            self.routine = '1q'
        # 2Q
        if (self.sp_y > self.loc.y) and (self.sp_x < self.loc.x):
            self.routine = '2q'
        # 3Q
        if (self.sp_y <= self.loc.y) and (self.sp_x <= self.loc.x):
            self.routine = '3q'
        # 4Q
        if (self.sp_y < self.loc.y) and (self.sp_x > self.loc.x):
            self.routine = '4q'
            



# MAIN
if __name__ == '__main__':
    # CRIATING OBJECT
    bsp = blastoiseSP()
    # ANGLES:
    a0 = 0.00
    a90 = piValue/2
    a180 = piValue
    a270 = (3*piValue)/2

    try:
        # LOOP
        while not rospy.is_shutdown():
            # 1 - CALL A SERVICE
            bsp.clt_getSP()
            # 2 - DEFINE YOUR ROTINE BY QUADRANTS
            bsp.sp_quadrantes_loc()
            # 3 - ROUNTINES TO FOLLOW BASED ON POINT QUADRANT
            if bsp.routine == '1q':
                # STEP I   - MOVE IN X
                bsp.movimente_linear_x()
                # STEP II  - ROTATE TO 90 DEGREES
                bsp.rotacione_angulo(a90)
                # STEP III - MOVE IN Y
                bsp.movimente_linear_y()
                # STEP IV  - ROTATE TO ORIGIN
                bsp.rotacione_angulo(a0)
                         
            elif bsp.routine == '2q':
                # STEP I   - ROTATE TO 90 DEGREES
                bsp.rotacione_angulo(a90)
                # STEP II  - MOVE IN Y
                bsp.movimente_linear_y()
                # STEP III - ROTATE TO 180 DEGREES
                bsp.rotacione_angulo(a180)
                # STEP IV  - MOVE IN X
                bsp.movimente_linear_x()
                # STEP V   - ROTATE TO ORIGIN
                bsp.rotacione_angulo(a0)
                
            elif bsp.routine == '3q':
                # STEP I   - ROTATE TO 180 DEGREES
                bsp.rotacione_angulo(a180)
                # STEP II  - MOVE IN X
                bsp.movimente_linear_x()
                # STEP III - ROTATE TO 270 DEGREES
                bsp.rotacione_angulo(a270)
                # STEP IV  - MOVE IN Y
                bsp.movimente_linear_y()
                # STEP V   - ROTATE TO ORIGIN
                bsp.rotacione_angulo(a0)

            elif bsp.routine == '4q': 
                # STEP I   - MOVE IN X
                bsp.movimente_linear_x()
                # STEP II  - ROTATE TO 270 DEGREES
                bsp.rotacione_angulo(a270)
                # STEP III - MOVE IN Y
                bsp.movimente_linear_y()
                # STEP IV  - ROTATE TO ORIGIN
                bsp.rotacione_angulo(a0)

            else:
                print 'ERROR - DO NOT HAVE ANY ROUTINE TO FOLLOW'

    except rospy.ROSInterruptException:
        pass