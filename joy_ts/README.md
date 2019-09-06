# ROS Tutorial - XBOX 360 Joystick control to Turtlesim

**Version**: Kinetic 1.12.14 OS: Ubuntu 16.04

**Overview**: Written in python, this package was created to control the turtle in turtlesim with a XBOX controller.

**Workflow**: Contains a Publisher/Subscriber node that takes coordinates from joystick and send to the turtlesim through topic communication. This solution (more in **Details**) solve the issue related to a continuos direction pressed in a buttom (callback function is not called, even if you are sending the turtle to some direction because the joystick does not send any sign).

**Example**: Moving the turtle with the joystick

![js](https://user-images.githubusercontent.com/32513366/57462661-a8b61980-724f-11e9-97ca-7cd33b63d6a0.gif)


### HOW TO RUN
(!) for each line open a new terminal in your system
```
  > roscore
  > sudo xboxdrv --detach-kernel-driver
  > rosrun joy joynode
  > rosrun turtlesim turtlesim_node
  > rosrun joy_ts joy_ctrl.py 
  
```
**PS**: If you have any communication problem between xboxdrv and joy (most because different USB input), use this line BEFORE start joy_node: ```> rosparam set joy_node/dev /dev/input/jsK```, where ```K``` is a number related to your USB input show by xboxdrv. 

**Details**: This package was based in Andrew Dai post: https://andrewdai.co/xbox-controller-ros.html#rosjoy. There you can find more information about joy and xbox driver.
