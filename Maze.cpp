
#include "Maze.h"
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
        // Remove unwanted characters
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) {
            return !std::isdigit(c) && c != ' ';
        }), line.end());

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

int Maze::at(int row, int col) const {
    
    if (row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size()) {
        return grid[row][col];
    }
    return -1; // Return -1 for invalid positions
}

std::pair<int, int> Maze::getSize() const {
    if (grid.empty()) return {0, 0};
    return {grid.size(), grid[0].size()};
}
