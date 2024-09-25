cmake_minimum_required(VERSION 3.10)

# Set project name
project(Project6CPlusPlus)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Webots include and link directories (assuming Webots is installed correctly on your system)
find_package(Webots REQUIRED)
find_package(JsonCpp REQUIRED)

# Include directories for header files
include_directories(
    include
    ${WEBOTS_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# Source files for the exploration controller
set(EXPLORATION_CONTROLLER_SOURCES
    src/exploration_controller.cpp
    src/exploration.cpp
    src/generate_noise.cpp
    src/geometry.cpp
    src/grid.cpp
    src/gui.cpp
    src/robot_gui.cpp
    src/robot.cpp
    src/utils.cpp
)

# Source files for the supervisor controller
set(SUPERVISOR_CONTROLLER_SOURCES
    supervisor_controller/supervisor_controller.cpp
)

# Define the executable for exploration controller
add_executable(exploration_controller ${EXPLORATION_CONTROLLER_SOURCES})

# Define the executable for supervisor controller
add_executable(supervisor_controller ${SUPERVISOR_CONTROLLER_SOURCES})

# Link the Webots library and JSON library to both executables
target_link_libraries(exploration_controller ${WEBOTS_LIBRARIES} ${JSONCPP_LIBRARIES})
target_link_libraries(supervisor_controller ${WEBOTS_LIBRARIES} ${JSONCPP_LIBRARIES})

# Install the binaries in the Webots controller directory (optional)
install(TARGETS exploration_controller supervisor_controller DESTINATION ${WEBOTS_PROJECT_DIR}/controllers)

# Set the Webots runtime path (important for finding Webots libraries at runtime)
set_target_properties(exploration_controller supervisor_controller PROPERTIES INSTALL_RPATH ${WEBOTS_LIBRARY_DIR})

# Print the message for successful configuration
message(STATUS "CMake configuration successful. You can now build the project.")
