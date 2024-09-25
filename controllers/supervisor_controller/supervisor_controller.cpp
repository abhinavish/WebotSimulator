#include <webots/Supervisor.hpp>
#include <webots/Robot.hpp>
#include <webots/Node.hpp>
#include <webots/Field.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <json/json.h>

using namespace webots;
using namespace std;

Supervisor* supervisor;

// Get floor dimensions
vector<float> get_floor_dim(Supervisor* supervisor) {
  Node* root_node = supervisor->getRoot();
  Field* children_field = root_node->getField("children");

  for (int i = 0; i < children_field->getCount(); ++i) {
    Node* node = children_field->getMFNode(i);
    if (node->getTypeName() == "RectangleArena") {
      Field* floor_size_field = node->getField("floorSize");
      if (floor_size_field) {
        const double* floor_size = floor_size_field->getSFVec2f();
        return {static_cast<float>(floor_size[0]), static_cast<float>(floor_size[1])};
      }
    }
  }
  return {0.0f, 0.0f};
}

// Get robot position
vector<float> get_robot_position(Supervisor* supervisor) {
  Node* robot_node = supervisor->getFromDef("e-puck"); // Replace with your robot's DEF name
  if (robot_node) {
    const double* translation = robot_node->getField("translation")->getSFVec3f();
    return {static_cast<float>(translation[0]), static_cast<float>(translation[1]), static_cast<float>(translation[2])};
  }
  return {0.0f, 0.0f, 0.0f};
}

// Get goal positions
vector<vector<float>> get_goal_position(Supervisor* supervisor) {
  Node* root_node = supervisor->getRoot();
  Field* children_field = root_node->getField("children");

  vector<vector<float>> goal_positions;
  for (int i = 0; i < children_field->getCount(); ++i) {
    Node* child_node = children_field->getMFNode(i);
    if (child_node->getTypeName().find("divergentindicator") != string::npos) {
      const double* translation = child_node->getField("translation")->getSFVec3f();
      cout << "Goal position: [" << translation[0] << ", " << translation[1] << "]" << endl;
      goal_positions.push_back({static_cast<float>(translation[0]), static_cast<float>(translation[1]), static_cast<float>(translation[2])});
    }
  }
  return goal_positions;
}

// Calculate wall corners
vector<vector<float>> calculate_wall_coordinates(const vector<float>& center, const vector<float>& rotation, const vector<float>& size, float padding = 0.05) {
  float cx = center[0], cy = center[1];
  float half_width = size[0] / 2.0;
  float half_length = size[1] / 2.0;
  float angle = rotation[3];

  vector<vector<float>> corners = {
    {cx - half_width - padding, cy - half_length - padding},
    {cx + half_width + padding, cy - half_length - padding},
    {cx + half_width + padding, cy + half_length + padding},
    {cx - half_width - padding, cy + half_length + padding}
  };

  vector<vector<float>> rotated_corners;
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);

  for (const auto& corner : corners) {
    float x = corner[0] - cx;
    float y = corner[1] - cy;
    float rotated_x = x * cos_angle - y * sin_angle + cx;
    float rotated_y = x * sin_angle + y * cos_angle + cy;
    rotated_corners.push_back({round(rotated_x * 100) / 100, round(rotated_y * 100) / 100});
  }

  return rotated_corners;
}

// Get wall positions
vector<vector<vector<float>>> get_wall_positions(Supervisor* supervisor, float padding = 0.05) {
  vector<vector<vector<float>>> wall_corners;
  Node* root_node = supervisor->getRoot();
  Field* children_field = root_node->getField("children");

  for (int i = 0; i < children_field->getCount(); ++i) {
    Node* child_node = children_field->getMFNode(i);
    if (child_node->getTypeName().find("wall") != string::npos) {
      const double* translation = child_node->getField("translation")->getSFVec3f();
      const double* rotation = child_node->getField("rotation")->getSFRotation();
      const double* size = child_node->getField("size")->getSFVec3f();
      vector<vector<float>> corners = calculate_wall_coordinates({translation[0], translation[1], translation[2]},
                                                                 {rotation[0], rotation[1], rotation[2], rotation[3]},
                                                                 {size[0], size[1], size[2]}, padding);
      wall_corners.push_back(corners);
    }
  }

  return wall_corners;
}

// Function to check if a point is inside a rotated rectangle
bool point_in_rotated_rectangle(const vector<float>& point, const vector<vector<float>>& rectangle) {
  // Check whether the point lies inside the given rectangle
  // Based on dot products
  return true;
}

// Function to generate map layout
vector<vector<string>> generate_map_of_grid_size(const vector<vector<vector<float>>>& walls, float width, float height, const vector<vector<float>>& goal_positions, float grid_size = 0.2) {
  vector<vector<string>> layout;
  float curr_x = -width / 2, curr_y = height / 2;

  while (curr_y >= -height / 2) {
    vector<string> row;
    while (curr_x <= width / 2) {
      if (/* check_if_in_wall(curr_x, curr_y, walls) */ false) {  // Use wall checking logic
        row.push_back("O");
      } else if (/* check_if_goal(curr_x, curr_y, goal_positions) */ false) {  // Use goal checking logic
        row.push_back("U");
      } else {
        row.push_back(".");
      }
      curr_x += grid_size;
    }
    layout.push_back(row);
    curr_x = -width / 2;
    curr_y -= grid_size;
  }

  return layout;
}

// Generate map JSON file
void generate_map_json(const vector<float>& robot_position, const vector<vector<vector<float>>>& walls, float width, float height, const vector<vector<float>>& goal_position, const vector<vector<string>>& layout, float grid_size, const string& world_title) {
  Json::Value map_data;
  map_data["width"] = static_cast<int>(width / grid_size);
  map_data["height"] = static_cast<int>(height / grid_size);
  map_data["cont_width"] = width;
  map_data["cont_height"] = height;
  map_data["start"] = {(robot_position[0] + width / 2) / grid_size, (-robot_position[1] + height / 2) / grid_size, 0};
  map_data["goals"] = Json::arrayValue;
  for (const auto& goal : goal_position) {
    Json::Value goal_pos;
    goal_pos.append(goal[0]);
    goal_pos.append(goal[1]);
    map_data["goals"].append(goal_pos);
  }
  map_data["layout"] = Json::arrayValue;
  for (const auto& row : layout) {
    Json::Value json_row(Json::arrayValue);
    for (const auto& cell : row) {
      json_row.append(cell);
    }
    map_data["layout"].append(json_row);
  }

  ofstream file("../exploration_controller/maps/" + world_title + ".json");
  file << map_data;
  file.close();
}

int main() {
  supervisor = new Supervisor();
  Node* root = supervisor->getRoot();
  Field* children_field = root->getField("children");
  Node* world_info = children_field->getMFNode(0);  // Assuming WorldInfo is the first child node
  string world_title = world_info->getField("title")->getSFString();

  int timestep = static_cast<int>(supervisor->getBasicTimeStep());

  vector<vector<vector<float>>> walls_RRT = get_wall_positions(supervisor, 0.04f);
  vector<vector<vector<float>>> walls_grid = get_wall_positions(supervisor, 0.04f);

  vector<float> robot_position = get_robot_position(supervisor);
  vector<vector<float>> goal_position = get_goal_position(supervisor);
  vector<float> floor_dim = get_floor_dim(supervisor);

  float grid_size = 0.04f;
  vector<vector<string>> layout = generate_map_of_grid_size(walls_grid, floor_dim[0], floor_dim[1], goal_position, grid_size);

  generate_map_json(robot_position, walls_RRT, floor_dim[0], floor_dim[1], goal_position, layout, grid_size, world_title);

  cout << "Map Generated for World: " << world_title << endl;

  return 0;
}
