#include "MazeUtils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility> // For std::pair
#include <limits>  // For std::numeric_limits
#include <algorithm> // For std::max_element
#include <unordered_map>


std::vector<std::vector<int>> readMaze(const std::string& fileName) {
    std::vector<std::vector<int>> maze;

    std::ifstream file(fileName);
    std::string line;

    // Read each line from the file and convert to a vector of integers.
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char ch : line) {
            if (ch >= '0' && ch <= '9') {
                row.push_back(ch - '0'); // Convert character to integer and add to row.
            }
        }
        maze.push_back(row); // Add row to the maze.
    }

    file.close();
    return maze; // Return the constructed maze.
}

void printMaze(const std::vector<std::vector<int>>& maze, const Agent& agent) {
    char agentSymbol;
    // Determine the symbol to represent the agent's direction.
    switch (agent.direction) {
        case NORTH: agentSymbol = '^'; break;
        case EAST:  agentSymbol = '>'; break;
        case SOUTH: agentSymbol = 'V'; break;
        case WEST:  agentSymbol = '<'; break;
    }

    // Iterate through the maze and print each cell.
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            // Print the agent symbol if the current cell is the agent's position.
            if (agent.position.x == i && agent.position.y == j) {
                // Validate if the agent's position is within the bounds of the maze.
                if (agent.position.x >= 0 && agent.position.x < maze.size() &&
                    agent.position.y >= 0 && agent.position.y < maze[i].size()) {
                    std::cout << agentSymbol << " "; // Display agent with its direction.
                } else {
                    // Handle out-of-bounds position.
                    std::cout << "X "; // Print 'X' or other symbol for invalid position.
                }
            } else {
                // Print the maze element for non-agent cells.
                std::cout << maze[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}