Research Track 1: first Assignment
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
