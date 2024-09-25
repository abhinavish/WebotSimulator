#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <utility>
#include <vector>

double grid_distance(double x1, double y1, double x2, double y2);
double diff_heading_deg(double heading1, double heading2);
std::pair<double, double> rotate_point(double x, double y, double heading_deg);
std::pair<double, double> find_centroid(const std::vector<std::pair<double, double>>& points);

#endif
