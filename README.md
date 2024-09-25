# Webots Robot Exploration & Supervisor - C++ Implementation

![Project Screenshot](./webot.jpeg)

This project contains the C++ implementation of two controllers for robot exploration and supervision in Webots. It simulates a robot navigating a maze-like environment, with the exploration controller guiding the robot's movements and the supervisor controller generating and managing the map of the environment.

## Project Structure

- **controllers/**
  - **exploration_controller/**
    - **include/**
      - exploration.hpp
      - grid.hpp
      - gui.hpp
      - robot_sim.hpp
      - utils.hpp
    - **src/**
      - exploration_controller.cpp
      - exploration.cpp
      - generate_noise.cpp
      - geometry.cpp
      - grid.cpp
      - gui.cpp
      - robot_gui.cpp
      - robot.cpp
      - utils.cpp
  - **supervisor_controller/**
    - supervisor_controller.cpp
  - **worlds/**
    - maze1.wbt
    - maze2.wbt
    - maze3.wbt


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
  ```
  

## Setup and Build Instructions

### 1. Install Webots
Ensure that Webots is installed and properly configured on your system. You can download it [here](https://cyberbotics.com/).

### 2. Clone the Repository
Clone this repository to your local machine:

```bash
git clone https://github.com/your-repository/WebotSimulator.git
cd WebotSimulator
```

## Build the Project

The project uses CMake to manage the build process.

### Step-by-Step Instructions:

1. **Create a build directory and navigate into it**:

    ```bash
    mkdir build
    cd build
    ```

2. **Run the CMake configuration**:

    ```bash
    cmake ..
    ```

3. **Build the project**:

    ```bash
    make
    ```

    The executables for the controllers will be created (`exploration_controller` and `supervisor_controller`).

---

## Run the Simulation

### 1. **Run the Exploration Controller**
To run the exploration controller in Webots, open one of the world files (e.g., `maze1.wbt`) in the Webots simulator, and attach the `exploration_controller` to the robot.

### 2. **Run the Supervisor Controller**
Similarly, for the supervisor controller, attach `supervisor_controller` to the supervisor node.

---

## Run the Controllers

Once you have built the executables:

- Run the **exploration controller** inside Webots by attaching the generated `exploration_controller` executable to your robot in the world.
- Run the **supervisor controller** similarly for overseeing the environment.
