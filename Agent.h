#ifndef AGENT_H
#define AGENT_H

#include <utility>
#include "Maze.h"
// For std::pair

using namespace std;








class Agent {
protected:
    pair<int, int> position;  // Agent's position in the maze

public:
    Agent(int row, int col) : position(row, col) {}
    virtual void move(const Maze &maze) = 0;  // Pure virtual function for movement
    pair<int, int> getPosition() const { return position; }
    void setPosition(int row, int col) { position.first = row; position.second = col; }
    std::vector<std::pair<std::pair<int, int>, int>> getNextPosition(const Maze &maze, std::pair<int, int> currentPosition);

    
    
};


class MyAgent : public Agent {
public:
    MyAgent(int row, int col) : Agent(row, col) {}

    void move(const Maze &maze) override {
        // Movement logic here
        // For example, you might decide to move right if possible
        pair<int, int> currentPosition = getPosition();
        setPosition(currentPosition.first, currentPosition.second + 1);
    }
};

#endif // AGENT_H
