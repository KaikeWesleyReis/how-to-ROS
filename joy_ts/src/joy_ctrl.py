#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

class ctrl_ts:
    def __init__(self):
        # INIT NODE AND TYPES
        rospy.init_node('ctrl_ts')
        self.pub = rospy.Publisher('turtle1/cmd_vel', Twist, queue_size=10)
        rospy.Subscriber('joy', Joy, self.cb, queue_size=10)
        # DEFINE OUR VARIABLE TO CONTROL OUR TURTLE
        self.move = Twist()

    def cb(self,joyMsg):
        # GET JOYSTICK COMMANDS
        self.move.linear.x = 4*joyMsg.axes[1]
        self.move.angular.z = 4*joyMsg.axes[0]


# MAIN
if __name__  == '__main__':
    try:
        cts = ctrl_ts()
        while not rospy.is_shutdown():
            cts.pub.publish(cts.move)

    except rospy.ROSInterruptException:
        pass