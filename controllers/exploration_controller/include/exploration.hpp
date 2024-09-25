#ifndef EXPLORATION_HPP
#define EXPLORATION_HPP

#include <utility>
#include "robot_sim.hpp"
#include "grid.hpp"

Robot_Sim* exploration_state_machine(Robot_Sim* robbie, Grid* grid);
Robot_Sim* frontier_planning(Robot_Sim* robbie, Grid* grid);

#endif
