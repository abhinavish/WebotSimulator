#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/GPS.hpp>
#include <webots/Compass.hpp>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include "grid.hpp"
#include "robot_sim.hpp"
#include "exploration.hpp"
#include "utils.hpp"
#include "gui.hpp"

#define TIME_STEP 64
#define MAX_SPEED 6.28

using namespace webots;
using namespace std;

class MoveRobot {
public:
    MoveRobot(Robot* robot, vector<pair<double, double>> path, int TIME_STEP, double MAX_SPEED) {
        this->robot = robot;
        this->path = path;
        this->TIME_STEP = TIME_STEP;
        this->MAX_SPEED = MAX_SPEED;

        this->step = 1;
        this->step_vis = 1;
        this->last_step_measured = 0;

        leftMotor = dynamic_cast<Motor*>(robot->getDevice("left wheel motor"));
        rightMotor = dynamic_cast<Motor*>(robot->getDevice("right wheel motor"));

        // Set initial wheel positions
        leftMotor->setPosition(INFINITY);
        rightMotor->setPosition(INFINITY);

        left_ps = dynamic_cast<PositionSensor*>(robot->getDevice("left wheel sensor"));
        right_ps = dynamic_cast<PositionSensor*>(robot->getDevice("right wheel sensor"));
        left_ps->enable(TIME_STEP);
        right_ps->enable(TIME_STEP);

        gps = dynamic_cast<GPS*>(robot->getDevice("gps"));
        gps->enable(TIME_STEP);

        compass = dynamic_cast<Compass*>(robot->getDevice("compass"));
        compass->enable(TIME_STEP);

        // Initialize motors
        leftMotor->setVelocity(0.0);
        rightMotor->setVelocity(0.0);
        robot->step(TIME_STEP);
    }

    SE2 get_robot_pose() {
        const double* gps_values = gps->getValues();
        const double* compass_values = compass->getValues();
        double x = gps_values[0];
        double y = gps_values[1];
        double h = atan2(compass_values[0], compass_values[1]);
        return SE2(x, y, h);
    }

    void move_forward(pair<double, double> next_coords, double speed = 5.0, double min_distance = 0.1) {
        cout << "Move forward..." << endl;
        while (robot->step(TIME_STEP) != -1) {
            rightMotor->setVelocity(speed * MAX_SPEED);
            leftMotor->setVelocity(speed * MAX_SPEED);

            SE2 pose = get_robot_pose();
            double current_dist = sqrt(pow(pose.x - next_coords.first, 2) + pow(pose.y - next_coords.second, 2));
            if (current_dist <= min_distance) {
                leftMotor->setVelocity(0.0);
                rightMotor->setVelocity(0.0);
                break;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    void turn_in_place(double expected_heading, double min_angle) {
        double speed = 0.1;

        while (robot->step(TIME_STEP) != -1) {
            SE2 pose = get_robot_pose();

            if (expected_heading < pose.h) {
                rightMotor->setVelocity(speed * MAX_SPEED * -1);
                leftMotor->setVelocity(speed * MAX_SPEED * 1);
            } else {
                rightMotor->setVelocity(speed * MAX_SPEED * 1);
                leftMotor->setVelocity(speed * MAX_SPEED * -1);
            }

            double heading_diff = fabs(pose.h - expected_heading);
            if (heading_diff < min_angle) {
                leftMotor->setVelocity(0.0);
                rightMotor->setVelocity(0.0);
                break;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    void move_to_next_coord(pair<double, double> node) {
        double min_distance = 0.1;
        double min_angle = 0.02;

        SE2 pose = get_robot_pose();
        double expected_heading = atan2(node.second - pose.y, node.first - pose.x);
        turn_in_place(expected_heading, min_angle);
        move_forward(node, 1.0, min_distance);
    }

    void follow_path() {
        for (const auto& node : path) {
            cout << "Moving to node: " << node.first << ", " << node.second << endl;
            move_to_next_coord(node);
        }
        cout << "Reached the goal!" << endl;
    }

private:
    Robot* robot;
    Motor* leftMotor;
    Motor* rightMotor;
    PositionSensor* left_ps;
    PositionSensor* right_ps;
    GPS* gps;
    Compass* compass;
    vector<pair<double, double>> path;
    int TIME_STEP;
    double MAX_SPEED;
    int step;
    int step_vis;
    int last_step_measured;
};

class RobotEnvController {
public:
    RobotEnvController(Robot_Sim* robbie, Grid* grid, MoveRobot* webot_robot, string program_state = "exploration") {
        this->robbie = robbie;
        this->grid = grid;
        this->webot_robot = webot_robot;
        this->program_state = program_state;
    }

    void update() {
        if (program_state == "exploration") {
            robbie = exploration_state_machine(robbie, grid);
        }
        update_motion();
    }

private:
    void update_motion() {
        robbie->dt = robbie->TIMESTEP;
        robbie->move_diff_drive(grid, robbie->vl, robbie->vr, robbie->dt);

        auto [c_x, c_y] = grid->discrete_to_cont(robbie->x, robbie->y);
        webot_robot->move_to_next_coord({c_x, c_y});
    }

    Robot_Sim* robbie;
    Grid* grid;
    MoveRobot* webot_robot;
    string program_state;
};

int main(int argc, char** argv) {
    Robot* robot = new Robot();
    int TIME_STEP = 64;
    double MAX_SPEED = 6.28;

    // Define map name and initialize grid
    string maze_name = "maze1";
    if (argc > 1) {
        maze_name = string(argv[1]);
    }

    string map_filename = "../exploration_controller/maps/" + maze_name + ".json";
    Grid grid(map_filename);

    // Initialize robot pose
    vector<double> robot_init_pose = grid.start;
    robot_init_pose.push_back(0.0); // 3D pose

    Robot_Sim robbie(robot_init_pose[0], robot_init_pose[1], robot_init_pose[2]);
    MoveRobot webot_robot(robot, {}, TIME_STEP, MAX_SPEED);

    RobotEnvController robot_env(&robbie, &grid, &webot_robot);
    robot_env.update();

    delete robot;
    return 0;
}
