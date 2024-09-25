#ifndef ROBOT_SIM_HPP
#define ROBOT_SIM_HPP

#include <vector>
#include <utility>
#include <set>

class Grid;

class Robot_Sim {
public:
    Robot_Sim(double x, double y, double heading = 0.0);

    std::pair<double, double> xy() const;
    double get_heading() const;
    
    void move_diff_drive(Grid* grid, double vl, double vr, double dt);
    std::vector<std::pair<int, int>> get_cells_in_fov(Grid* grid, double dist = 10);
    std::vector<std::pair<int, int>> get_obstacles_in_fov(Grid* grid, double dist = 10);

    std::set<std::pair<int, int>> explored_cells;
    std::vector<std::pair<double, double>> path;
    std::vector<std::pair<int, int>> markers_found_or_picked;
    double x, y, h, wheel_dist, wheel_r;
    double vl, vr, TIMESTEP;

    std::pair<double, double> next_coord;
};

#endif
