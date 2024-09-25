# Webots Robot Exploration & Supervisor - C++ Implementation

This project contains the C++ implementation of two controllers for robot exploration and supervision in Webots. It simulates a robot navigating a maze-like environment, with the exploration controller guiding the robot's movements and the supervisor controller generating and managing the map of the environment.

## Project Structure

```plaintext
controllers/
│
├── exploration_controller/
│   ├── include/                   # Header files for the exploration controller
│   │   ├── exploration.hpp
│   │   ├── grid.hpp
│   │   ├── gui.hpp
│   │   ├── robot_sim.hpp
│   │   ├── utils.hpp
│   ├── src/                       # Source files for the exploration controller
│   │   ├── exploration_controller.cpp
│   │   ├── exploration.cpp
│   │   ├── generate_noise.cpp
│   │   ├── geometry.cpp
│   │   ├── grid.cpp
│   │   ├── gui.cpp
│   │   ├── robot_gui.cpp
│   │   ├── robot.cpp
│   │   ├── utils.cpp
│
├── supervisor_controller/         # Source for supervisor controller
│   ├── supervisor_controller.cpp
│
├── worlds/                        # Webots world files
│   ├── maze1.wbt
│   ├── maze2.wbt
│   ├── maze3.wbt
│   ├── .maze1.jpg
│   ├── .maze2.jpg
│   ├── .maze3.jpg
│
├── CMakeLists.txt                 # CMake configuration file for building the project
└── README.md                      # This README file
