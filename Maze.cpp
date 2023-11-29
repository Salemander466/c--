
#include "Maze.h"
#include "Agent.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

void Maze::loadMaze(const std::string& filename) {
    std::ifstream file(filename);
    grid.clear();

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Print the original line
        std::cout << "Original line: " << line << std::endl;

        // Remove unwanted characters
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) {
            return !std::isdigit(c) && c != ' ';
        }), line.end());

        // Print the processed line
        std::cout << "Processed line: " << line << std::endl;

        std::vector<int> row;
        std::stringstream ss(line);
        int num;
        while (ss >> num) {
            row.push_back(num);
        }
        grid.push_back(row);
    }

    // Print the grid to verify its contents
    std::cout << "Loaded maze grid:" << std::endl;
    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void Maze::printMaze() {
    std::cout << "Maze grid:" << std::endl;
    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}


int Maze::at(int row, int col) const {
    
    if (row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size()) {
        return grid[row][col];
    }
    return -1; // Return -1 for invalid positions
}

std::pair<int, int> Maze::getSize() const {
    if (grid.empty()) {
        std::cerr << "Error: The maze grid is empty." << std::endl;
        return {0, 0};
    }
    
    int rows = grid.size();
    int cols = grid.empty() ? 0 : grid[0].size(); // Check if grid is empty before accessing

    // Debugging: Print the size
    // std::cout << "Maze Size: Rows = " << rows << ", Cols = " << cols << std::endl;

    return std::make_pair(rows, cols);
}

std::pair<int, int> Maze::findNumberCoordinates(int number) const {
    for (int row = 0; row < grid.size(); ++row) {
        for (int col = 0; col < grid[row].size(); ++col) {
            if (grid[row][col] == number) {
                return std::make_pair(row, col);  // Return the coordinates if number is found
            }
        }
    }
    return std::make_pair(-1, -1);  // Return (-1, -1) if the number is not found
}
