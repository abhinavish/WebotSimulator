#include <cstdlib>
#include <ctime>
#include <cmath>
#include "robot_sim.hpp"

#define OFFSET 0.01

// Function to generate basic noise
double basic_noise(double v) {
    // Generate a small random noise and add to the velocity
    return v + static_cast<double>(rand()) / RAND_MAX * 0.01;
}

// Function to add noise to robot's wheel velocities
void add_noise(Robot_Sim* robbie) {
    robbie->vl = basic_noise(robbie->vl);
    robbie->vr = basic_noise(robbie->vr);
}

// Function to add offset noise to robot's wheel velocities
void add_offset_noise(Robot_Sim* robbie) {
    robbie->vl = offset_noise(robbie->vl);
    robbie->vr = offset_noise(robbie->vr);
}

// Function to generate offset noise
double offset_noise(double v) {
    return v + OFFSET;
}
