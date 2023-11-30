#ifndef MAZEUTILS_H
#define MAZEUTILS_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility> // For std::pair
#include <limits>  // For std::numeric_limits
#include <algorithm> // For std::max_element
#include <unordered_map>
#include "Agent.h" // Include the Agent header if you need the Agent structure in these functions

struct Agent;
// Declaration of function for reading a maze from a file
std::vector<std::vector<int>> readMaze(const std::string& fileName);

// Declaration of function for printing the maze with the agent's position
void printMaze(const std::vector<std::vector<int>>& maze, const Agent& agent);

#endif // MAZEUTILS_H
