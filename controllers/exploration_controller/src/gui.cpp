#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <cmath>
#include "grid.hpp"
#include "robot_sim.hpp"
#include "utils.hpp"

class GUIWindow {
public:
    GUIWindow(Grid* grid, std::string program_state, std::mutex& lock)
        : grid(grid), program_state(program_state), lock(lock) {
        width = grid->width;
        height = grid->height;
        scale = 20;  // Adjust this for grid scaling
        robot = nullptr;
    }

    // Main method to start GUI
    void start(std::atomic<bool>& stop_event) {
        sf::RenderWindow window(sf::VideoMode(width * scale, height * scale), "Warehouse: Red - robot");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            if (stop_event) {
                window.close();
            }

            // Drawing the grid and objects
            window.clear(sf::Color(97, 97, 97));  // Gray background
            drawFreeCells(window);
            drawOccupied(window);
            drawMarkers(window);
            drawCentroid(window);

            if (robot != nullptr) {
                show_robot(window, robot);
            }

            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    // Update robot instance for the GUI thread
    void show_robot(Robot_Sim* robbie) {
        std::lock_guard<std::mutex> guard(lock);
        robot = robbie;
    }

private:
    Grid* grid;
    Robot_Sim* robot;
    std::string program_state;
    int width, height, scale;
    std::mutex& lock;

    // Drawing individual cells
    void drawFreeCells(sf::RenderWindow& window) {
        sf::RectangleShape cell(sf::Vector2f(scale, scale));
        cell.setFillColor(sf::Color::White);

        for (const auto& block : grid->empty) {
            cell.setPosition(block.first * scale, block.second * scale);
            window.draw(cell);
        }
    }

    // Drawing occupied cells
    void drawOccupied(sf::RenderWindow& window) {
        sf::RectangleShape cell(sf::Vector2f(scale, scale));
        cell.setFillColor(sf::Color(34, 34, 34));  // Dark gray

        for (const auto& block : grid->occupied) {
            cell.setPosition(block.first * scale, block.second * scale);
            window.draw(cell);
        }
    }

    // Drawing markers
    void drawMarkers(sf::RenderWindow& window) {
        if (program_state == "exploration" && robot) {
            for (const auto& marker : grid->markers) {
                if (robot->explored_cells.count({marker.first, marker.second}) == 0) {
                    continue;
                }
                drawMarker(window, marker);
            }
        }
    }

    // Drawing individual marker
    void drawMarker(sf::RenderWindow& window, const std::tuple<int, int, char>& marker) {
        sf::RectangleShape markerRect(sf::Vector2f(scale, scale));
        markerRect.setFillColor(sf::Color(0, 140, 140));  // Cyan

        int x = std::get<0>(marker);
        int y = std::get<1>(marker);
        char dir = std::get<2>(marker);

        markerRect.setPosition(x * scale, y * scale);
        window.draw(markerRect);

        // Drawing directional arrows
        sf::VertexArray arrow(sf::Lines, 2);
        if (dir == 'U') {
            arrow[0].position = sf::Vector2f((x + 0.5) * scale, y * scale);
            arrow[1].position = sf::Vector2f((x + 0.5) * scale, (y - 1) * scale);
        } else if (dir == 'D') {
            arrow[0].position = sf::Vector2f((x + 0.5) * scale, (y + 1) * scale);
            arrow[1].position = sf::Vector2f((x + 0.5) * scale, y * scale);
        } else if (dir == 'L') {
            arrow[0].position = sf::Vector2f(x * scale, (y + 0.5) * scale);
            arrow[1].position = sf::Vector2f((x - 1) * scale, (y + 0.5) * scale);
        } else if (dir == 'R') {
            arrow[0].position = sf::Vector2f((x + 1) * scale, (y + 0.5) * scale);
            arrow[1].position = sf::Vector2f(x * scale, (y + 0.5) * scale);
        }
        arrow[0].color = sf::Color::Black;
        arrow[1].color = sf::Color::Black;

        window.draw(arrow);
    }

    // Drawing centroid
    void drawCentroid(sf::RenderWindow& window) {
        if (grid->centroid.first == 0 && grid->centroid.second == 0) return;

        sf::CircleShape centroid(5);  // Small dot for centroid
        centroid.setFillColor(sf::Color(0, 145, 234));  // Blue
        centroid.setPosition(grid->centroid.first * scale - 5, grid->centroid.second * scale - 5);
        window.draw(centroid);
    }

    // Drawing the robot
    void show_robot(sf::RenderWindow& window, Robot_Sim* robbie) {
        sf::CircleShape robotShape(10);  // Robot size
        robotShape.setFillColor(sf::Color::Red);  // Robot in red
        robotShape.setPosition(robbie->x * scale - 10, robbie->y * scale - 10);
        window.draw(robotShape);

        // Draw line from robot to next coordinate
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(robbie->x * scale, robbie->y * scale)),
            sf::Vertex(sf::Vector2f(robbie->next_coord.first * scale, robbie->next_coord.second * scale))
        };
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::Lines);
    }
};
