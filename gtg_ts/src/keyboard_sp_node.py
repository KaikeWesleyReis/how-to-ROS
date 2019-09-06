#!/usr/bin/env python
import rospy
from control_turtle_setpos.srv import getSetPosKeyboard
from std_msgs.msg import Float32

# SERVICE FUNCTION
def get_kb_axis(hold_arg):
    while 1:
        print 'Boundaries: 0 <= x,y <= 11.05'
        x = float(input('Insert your X Coordinate: '))
        y = float(input('Insert your Y Coordinate: '))
        if x > 11.05 or x < 0.00 or y > 11.05 or y < 0.00:
            print '!!! Out of Boundaries, Insert AGAIN !!!'
        else:
            print '-----'
            return x,y    

# MAIN
if __name__ == '__main__':
    rospy.init_node('srv_getSP_node')
    try:
        while not rospy.is_shutdown():
            rospy.Service('srv_getSP', getSetPosKeyboard, get_kb_axis)
            rospy.spin()
    except rospy.ROSInterruptException:
        pass
