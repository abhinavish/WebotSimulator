#include <cmath>
#include <vector>

double grid_distance(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

double diff_heading_deg(double heading1, double heading2) {
    double dh = heading1 - heading2;
    while (dh > 180) dh -= 360;
    while (dh <= -180) dh += 360;
    return dh;
}

std::pair<double, double> rotate_point(double x, double y, double heading_deg) {
    double c = std::cos(heading_deg * M_PI / 180);
    double s = std::sin(heading_deg * M_PI / 180);
    double xr = x * c + y * s;
    double yr = -x * s + y * c;
    return {xr, yr};
}

std::pair<double, double> find_centroid(const std::vector<std::pair<double, double>>& points) {
    double sum_x = 0, sum_y = 0;
    for (const auto& p : points) {
        sum_x += p.first;
        sum_y += p.second;
    }
    return {sum_x / points.size(), sum_y / points.size()};
}
