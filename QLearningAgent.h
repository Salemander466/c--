#ifndef QLEARNINGAGENT_H
#define QLEARNINGAGENT_H

#include "Agent.h"
#include "Maze.h"  // Assuming Maze class is defined in Maze.h

class QLearningAgent : public Agent {
    int calculateReward(const Maze &maze, int row, int col);
private:
    double Q[10][10][4]; // Q-table for states and actions
    int stepsTaken;
    std::pair<int, int> startingPosition;
    const double ALPHA = 0.1;  // Learning rate
    const double GAMMA = 0.9;  // Discount factor
    const double EPSILON = 0.1;  // Exploration rate
    const int MAX_STEPS = 100;  // Maximum steps per episode
    const int GOAL_REWARD = 10;  // Reward for reaching the goal
    const int STEP_REWARD = -1;  // Penalty for each step
    const int WALL_REWARD = -5;  // Penalty for hitting a wall
    // QLearningAgent.h
    int lastAction = -1; 
    // ... other includes and declarations ...

    // Constants for rewards and penalties
    const int WALL_PENALTY = -10;
    const int GOGGLES_REWARD = 5;
    const int SPEED_POTION_REWARD = 5;
    const int FOG_PENALTY = -5;
    const int SLOWPOKE_POTION_PENALTY = -5;
    std::pair<int, int> calculateNewPosition(std::pair<int, int> currentPosition, int action);
    // ... QLearningAgent class declaration ...
    std::vector<std::pair<std::pair<int, int>, int>> validMoves;
    
    std::vector<std::pair<std::pair<int, int>, int>> getNextPosition(const Maze &maze, std::pair<int, int> currentPosition);

    
    int direction;  // 0: up, 1: right, 2: down, 3: left
    int speed;
    
public:
    QLearningAgent(int row, int col);
    int chooseAction(const Maze &maze);
    void updateQValues(int action, int reward, int newRow, int newCol);
    void move(const Maze &maze);
    void reset();  // Resets the agent to the starting position
    bool hasReachedGoal(const Maze &maze);  // Checks if the agent has reached the goal, now taking Maze as a parameter
    bool isValidMove(const Maze &maze, std::pair<int, int> newPosition);
    std::pair<int, int> getNextPosition(const Maze &maze, std::pair<int, int> currentPosition, int action, int lastAction);
    int mapPositionToAction(std::pair<int, int> currentPosition, std::pair<int, int> newPosition);
    // ...

};

#endif // QLEARNINGAGENT_H
