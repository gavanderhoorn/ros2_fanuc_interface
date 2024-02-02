# ros2_fanuc_interface

This package implements ros2 hardware interface for the CRX family Fanuc robots. 


## Installation on the remote PC


### Prerequisites - EIPScanner
The communication between the external pc and the Fanuc controller happens by means of the [Ethernet/IP](https://en.wikipedia.org/wiki/EtherNet/IP) protocol. 
In particular, this package relies on the implementation provided in [EIPScanner](https://eipscanner.readthedocs.io/en/latest/).  
Please, follow the instructoins [here](https://eipscanner.readthedocs.io/en/latest/getting_started.html#installing) to install it.

NOTE: a previous version of this driver used a python driver for the Ethernet/IP communication. Please refer to the [python-driver](https://github.com/paolofrance/ros2_fanuc_interface/tree/python-driver) branch.

### Package installation

To install the current package just
```console
$ cd <to-your-src>
$ git clone https://github.com/paolofrance/ros2_fanuc_interface
```

To use it with the [ros2_control](https://control.ros.org/master/index.html) framework, additional [Moveit!](https://moveit.picknik.ai/main/index.html) and description packages are required. Download the packages
```console
$ git clone https://github.com/paolofrance/crx20_moveit_config
$ git clone https://github.com/paolofrance/crx_description
$ cd ..
$ colcon build --symlink-install
```

The Moveit! package is currently available for the [crx-20ial](https://www.fanuc.eu/ch/it/robot/robot-filter-page/robot-collaborativi/crx-20ial) robot only.

If some error appears, please check the Moveit! and ros2_control installation.
```console
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install ros-<distro>-moveit
$ sudo apt install ros-<distro>-ros2-control
$ sudo apt install ros-<distro>-ros2-controllers
```

## Installation on the robot controller

### TP program installation

To actually move the robot, an easy teach-pendant (TP) program is required. The current version of the driver does not include it, but will be included in future releases.

### DPM input setup (optional)

To use the DPM (Dynamic Path Modification) module provided by the Fanuc robots, it is necessary to configure 6 Group Inputs and map them to the DPM settings.

## Usage

This section explains what this package allows at the current status

#### Real robot

To use it open a terminal and do the following command:

```console
$ ros2 launch ros2_fanuc_interface robot_bringup.launch.py robot_ip:=your.robot.ip.address
```
#### Real robot - read only

It is sometimes necessary to move the robot from the Teach Pendant, or via manual guidance, but still required to read the joint states (e,g,. Kinestetic teaching). This feature requires no programs running on the TP, and an additional parameter during launch. 
To use it open a terminal and do the following command:

```console
$ ros2 launch ros2_fanuc_interface robot_bringup.launch.py robot_ip:=your.robot.ip.address read_only:=true
```

**NOTE**: with some version of the robot controller software we had some issues related to loss of communication. It is still unclear the reason. 

#### Fake robot

To test this with mock components just add the "use_mock_hardware:=true" param to your launch command
```console
$ ros2 launch ros2_fanuc_interface robot_bringup.launch.py use_mock_hardware:=true
```

#### Trajectory execution velocity scaling

It is also possible to control the robot allowing dynamic velocity scaling of the trajectory under execution.
This feature is unrelated to this package but is a useful tool that can be used for example to dynamically scale the velocity according to the distance between human and robot.

To test the velocity scaling controller you have to download the [scaled_fjt_controller](https://github.com/paolofrance/scaled_fjt_controller). 

If you want to test it along with the velocity scaler controller, open a terminal and 

```console
ros2 launch ros2_fanuc_interface robot_bringup.launch.py controllers_file:=scaled_velocity_controller.yaml
```

You should now see the "/speed_ovr" topic, where you can publish the desired velocity (as a percentage of the maximum velocity). See the documentation [here](https://github.com/paolofrance/scaled_fjt_controller).

## Known issues
1. execution delay of about 0.2 seconds - reduced compared to the previous version with python

## TODO
list of known todos and desiderata:  
1. implementation of the Fanuc DPM to allow faster control for contact tasks
2. implementation of the Fanuc RMI to remove the need for TP programs

## Contacts
Ros developer: paolo.franceschi@supsi.ch  
TP fanuc developer: stefano.baraldo@supsi.ch  
Tester and user: vincenzo.pomponi@supsi.ch  

### Acknowledgements
This package is developed by the [ARM (Automation Robotics and Machines Laboratory)](https://sites.supsi.ch/isteps_en/Laboratories/gruppo1.html) at SUPSI, Lugano, CH.
The EU project [Fluently](https://www.fluently-horizonproject.eu/) partially funded the development of this package.

