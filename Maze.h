#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Maze {
protected:
    std::pair<int, int> position;  // Agent's position in the maze

private:
    std::vector<std::vector<int> > grid; // 2D vector to store the maze

public:
    // Constructor that loads a maze from a file
    Maze(const std::string& filename) {
        loadMaze(filename);
    }
    std::pair<int, int> findNumberCoordinates(int number) const;
    // Load maze from a file
    void loadMaze(const std::string& filename);
    void printMaze();
    // Get the value at a specific position in the maze
    int at(int row, int col) const;

    // Get the size of the maze
    std::pair<int, int> getSize() const;




    
    // Additional methods...
};

#endif // MAZE_H
