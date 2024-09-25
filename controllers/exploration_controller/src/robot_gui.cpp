#include <thread>
#include <chrono>
#include "grid.hpp"
#include "robot_sim.hpp"
#include "exploration.hpp"
#include "utils.hpp"
#include "generate_noise.hpp"

class RobotEnv {
public:
    RobotEnv(Robot_Sim* robbie, Grid* grid, std::string program_state = "exploration", bool testing = false)
        : robbie(robbie), grid(grid), program_state(program_state), testing(testing) {
        robbie->markers_found_or_picked = robbie->read_marker_around(grid);
    }

    void update() {
        if (program_state == "exploration") {
            robbie = exploration_state_machine(robbie, grid);
        } else {
            std::cout << "Unrecognized program state" << std::endl;
        }
        update_motion();
    }

private:
    void update_motion() {
        robbie->dt = robbie->TIMESTEP;
        robbie->move_diff_drive(grid, robbie->vl, robbie->vr, robbie->dt);

        if (program_state == "exploration") {
            auto marker_list = robbie->read_marker_around(grid);
            if (robbie->markers_found_or_picked.size() != 
                union_set(robbie->markers_found_or_picked, marker_list).size()) {
                robbie->markers_found_or_picked = union_set(robbie->markers_found_or_picked, marker_list);
                std::cout << grid->fname << " found " << robbie->markers_found_or_picked.size() 
                          << "/" << grid->LANDMARKS_TOTAL << " markers" << std::endl;
            }
        }
    }

    Robot_Sim* robbie;
    Grid* grid;
    std::string program_state;
    bool testing;
};

// Thread to run the robot environment
class RobotEnvThread {
public:
    RobotEnvThread(RobotEnv* robot_env, bool testing = false, double time_limit = std::numeric_limits<double>::infinity())
        : robot_env(robot_env), testing(testing), time_limit(time_limit), start_time(std::chrono::system_clock::now()) {}

    void run() {
        while (true) {
            auto now = std::chrono::system_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
            robot_env->update();

            if (elapsed_time > time_limit) {
                std::cout << "Run time exceeds " << time_limit << " seconds." << std::endl;
                break;
            }

            if (robot_env->robbie->markers_found_or_picked.size() == robot_env->grid->LANDMARKS_TOTAL) {
                std::cout << "Found all markers. Good job!" << std::endl;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

private:
    RobotEnv* robot_env;
    bool testing;
    double time_limit;
    std::chrono::system_clock::time_point start_time;
};
