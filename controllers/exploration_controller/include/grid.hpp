#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <string>
#include <utility>

class Grid {
public:
    Grid(const std::string& filename);
    
    bool is_in(int x, int y) const;
    bool is_occupied(int x, int y) const;
    bool is_collision_with_obstacles(const std::pair<int, int>& p1, const std::pair<int, int>& p2) const;

    std::pair<double, double> start;
    std::vector<std::pair<int, int>> occupied, empty, markers;
    std::pair<int, int> centroid;

    int width, height, LANDMARKS_TOTAL;
    std::string fname;

private:
    void load_grid(const std::string& filename);
};

#endif
