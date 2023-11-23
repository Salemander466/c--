#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Maze {
private:
    std::vector<std::vector<int>> grid; // 2D vector to store the maze

public:
    // Constructor
    Maze(const std::string& filename) {
        loadMaze(filename);
    }

    // Load maze from a file
    void loadMaze(const std::string& filename) {
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

    // Get the value at a specific position in the maze
    int at(int row, int col) const {
        if (row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size()) {
            return grid[row][col];
        }
        return -1; // Return -1 for invalid positions
    }

    // Get the size of the maze
    std::pair<int, int> getSize() const {
        if (grid.empty()) return {0, 0};
        return {grid.size(), grid[0].size()};
    }

    // Add more methods if needed...
};

#endif // MAZE_H
