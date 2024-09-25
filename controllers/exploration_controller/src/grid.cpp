#include <string>
#include <fstream>
#include <json/json.h>
#include "utils.hpp"

class Grid {
public:
    Grid(const std::string& fname) {
        std::ifstream file(fname);
        Json::Value config;
        file >> config;

        width = config["width"].asInt();
        height = config["height"].asInt();
        start = {config["start"][0].asDouble(), config["start"][1].asDouble()};

        for (const auto& row : config["layout"]) {
            for (const auto& entry : row) {
                // Process map entries: empty, occupied, etc.
            }
        }
    }

    std::vector<std::pair<int, int>> occupied, empty, markers;
    std::pair<double, double> start;
    int width, height, LANDMARKS_TOTAL = 5;
    std::string fname;

    bool is_in(int x, int y) const {
        // Check if within grid bounds
    }

    bool is_collision_with_obstacles(const std::pair<int, int>& p1, const std::pair<int, int>& p2) const {
        // Check for obstacle collision
    }
};
