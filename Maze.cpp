// Maze.cpp
#include "Maze.h"
#include <fstream>
#include <iostream>

void Maze::loadMaze(const std::string& filename) {
    std::ifstream file(filename);
    grid.clear();

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char ch : line) {
            switch (ch) {
                case '#': row.push_back(1); break; // Wall
                case '.': row.push_back(0); break; // Path
                // Add more cases for other types of cells if needed
                default: row.push_back(0); break; // Default to path
            }
        }
        grid.push_back(row);
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
