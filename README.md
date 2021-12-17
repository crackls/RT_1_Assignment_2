Research Track 1: second Assignment
================================

Here in this repository you can find the c++ code for the second assignment of the course Research Track 1,
done by Carlos Ángel López de Rodas Serrano. 


Running
----------------------
First run  the simulation environment with:
```bash
$ prosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
Then, to execute the nodes use the following comands:
* To run the robot controller node use:
```bash
$ rosrun assignment_2 bot_cont_node
```
* To run the user interface node use:
```bash
$ rosrun assignment_2 user_interface_node
```

## Code description
-----------------------------

There are two mains codes:
* A code for autonomous driving of a robot in a circuit, robot controller node (``bot_cont_node``)
* A code that will let the user control the velocity of the robot adn reset its position using the keyboard arrows, the user interface node (``user_interfate_node``)

### Robot controller node ###

The logic behind this node is the following:

```
ros::init("bot_controller_node");

ros::subscribe(/base_scan);
IF (obstacles ahead);
	IF (obstacles on the left are closer than in the right);
		turn right;
	ELSE;
		turn left;
		
ros::publish(/cmd_vel);

ros::service(/inc_dec);
SWITCH (request from the service):
	case 1:
		increase the velocity;
	case 2:
		decrease the velocity;

ros::spin();
```

This code is subscribing to the topic ``/base_scan`` which is publushing info about the robot position in the circuit. 
It is also publishing the velocity in order to control the robot in the circuit.
It also implements a funcionality to increase or decrease the velocity using the service ``/inc_dec``.

### User interface node ###

The logic behind this node is the following:

```
ros::init("user_interface_node");

ros::client(/inc_dec);

ros::client(/reset_positions);

sets the console in raw mode
FOR(;;);
	waits for keyboard input
	SWITCH (key pressed);
		case arrow_up:
			call inc_dec.server.request = 1;
		case arrow_down:
			call inc_dec.server.request = 2;
		case arrow_left:
			call resset_positions.server;
```

This code has two clients, one to request to change the velocity (``/inc_dec``) and the other one to request the robot to teleport to its initial position (``reset_positions``).






















