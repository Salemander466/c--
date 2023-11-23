#ifndef QLEARNINGAGENT_H
#define QLEARNINGAGENT_H

#include "Agent.h"
#include "Maze.h"  // Assuming Maze class is defined in Maze.h

class QLearningAgent : public Agent {
private:
    double Q[10][10][4]; // Q-table for states and actions
    int stepsTaken;
    pair<int, int> startingPosition;
    const double ALPHA = 0.1;  // Learning rate
    const double GAMMA = 0.9;  // Discount factor
    const double EPSILON = 0.1;  // Exploration rate
    const int MAX_STEPS = 100;  // Maximum steps per episode
    const int GOAL_REWARD = 10;  // Reward for reaching the goal
    const int STEP_REWARD = -1;  // Penalty for each step
    const int WALL_REWARD = -5;  // Penalty for hitting a wall

public:
    QLearningAgent(int row, int col);
    int chooseAction(const Maze &maze);
    void updateQValues(int action, int reward, int newRow, int newCol);
    void move(const Maze &maze);
    void reset();  // Resets the agent to the starting position
    bool hasReachedGoal();  // Checks if the agent has reached the goal
};

#endif // QLEARNINGAGENT_H
