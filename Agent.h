#ifndef AGENT_H
#define AGENT_H

#include <utility>  // For std::pair

using namespace std;

class Agent {
protected:
    pair<int, int> position;  // Agent's position in the maze

public:
    Agent(int row, int col) : position(row, col) {}
    virtual void move(const Maze &maze) = 0;  // Pure virtual function for movement
    pair<int, int> getPosition() const { return position; }
    void setPosition(int row, int col) { position.first = row; position.second = col; }
};

#endif // AGENT_H
