# Webots Robot Exploration & Supervisor - C++ Implementation

This project contains the C++ implementation of two controllers for robot exploration and supervision in Webots. It simulates a robot navigating a maze-like environment, with the exploration controller guiding the robot's movements and the supervisor controller generating and managing the map of the environment.

## Project Structure

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

## Key Components

### 1. **Exploration Controller**
The exploration controller handles the robot's pathfinding and movement within the maze environment. It is structured using C++ and the Webots API.

- **exploration_controller.cpp**: Main entry point for the robot's exploration logic.
- **exploration.cpp**: Implements exploration algorithms such as frontier-based exploration.
- **generate_noise.cpp**: Adds noise to the robot's sensor data to simulate real-world conditions.
- **geometry.cpp**: Handles geometric calculations for robot movement.
- **grid.cpp**: Manages the grid-based environment representation.
- **gui.cpp**: Handles the graphical user interface for visualizing the environment.
- **robot_gui.cpp**: Combines robot and GUI functionality.
- **robot.cpp**: Simulates the robot's physical model and controls.
- **utils.cpp**: Contains utility functions such as distance calculations and transformations.

### 2. **Supervisor Controller**
The supervisor controller oversees the entire simulation, manages the environment (walls, goals, etc.), and generates the map of the world in a JSON format.

- **supervisor_controller.cpp**: Retrieves robot position, wall locations, and goal positions from the Webots simulation, and exports them into a `.json` map file.

### 3. **Worlds**
The `worlds/` directory contains the Webots world files for simulating the maze environments. Each world file (`.wbt`) corresponds to a different maze layout, and images of the mazes are also provided.

---

## Dependencies

This project requires the following dependencies:

- **Webots**: Webots is a free open-source robot simulator used for this project. [Download Webots](https://cyberbotics.com/)
- **C++11 or higher**: The project uses features from C++11.
- **JsonCpp**: A C++ library for interacting with JSON files. Install it with:
  ```bash
  sudo apt-get install libjsoncpp-dev

