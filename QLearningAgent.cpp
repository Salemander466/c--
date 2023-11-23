#include "QLearningAgent.h"
#include <utility> // For std::pair
#include <cstdlib> // For std::rand, std::srand
#include <ctime> // For std::time

QLearningAgent::QLearningAgent(int row, int col) : Agent(row, col) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed for randomness
    // Initialize Q-table to 0
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 4; k++) {
                Q[i][j][k] = 0;
            }
        }
    }
    stepsTaken = 0;
    startingPosition = std::make_pair(row, col);
}

int QLearningAgent::chooseAction(const Maze &maze) {
    // Epsilon-greedy strategy
    if ((rand() % 100) / 100.0 < EPSILON) {
        return rand() % 4; // Random action (0 up, 1 right, 2 down, 3 left)
    } else {
        // Choose the best action based on Q-values
        int bestAction = 0;
        double bestValue = Q[position.first][position.second][0];
        for (int a = 1; a < 4; a++) {
            if (Q[position.first][position.second][a] > bestValue) {
                bestValue = Q[position.first][position.second][a];
                bestAction = a;
            }
        }
        return bestAction;
    }
}


void QLearningAgent::updateQValues(int action, int reward, int newRow, int newCol) {
    // Find the maximum Q-value for the new state
    double maxQNew = *max_element(Q[newRow][newCol], Q[newRow][newCol] + 4);

    // Update Q-value using the Q-learning formula
    Q[position.first][position.second][action] += ALPHA * (reward + GAMMA * maxQNew - Q[position.first][position.second][action]);
}


void QLearningAgent::move(const Maze &maze) {
    int action = chooseAction(maze);
    
    // Calculate new position based on action
    int newRow = position.first;
    int newCol = position.second;
    switch (action) {
        case 0: newRow--; break; // Up
        case 1: newCol++; break; // Right
        case 2: newRow++; break; // Down
        case 3: newCol--; break; // Left
    }

    // Check for boundaries and walls
    if (newRow < 0 || newRow >= maze.size() || newCol < 0 || newCol >= maze[0].size() || maze[newRow][newCol] == 1) {
        newRow = position.first;
        newCol = position.second;
    }

    int reward = STEP_REWARD; // Default reward
    if (maze[newRow][newCol] == 2) {  // Found goggles
        // Increase perceptive field
        reward = 5;  // Positive reward for finding goggles
    } else if (maze[newRow][newCol] == 3) {  // Found speed potion
        // Increase step width
        reward = 5;  // Positive reward for finding speed potion
    } else if (newRow == GOAL_ROW && newCol == GOAL_COL) {
        reward = GOAL_REWARD; // Reward for reaching the goal
    }

    updateQValues(action, reward, newRow, newCol);
    position.first = newRow;
    position.second = newCol;

    // Check for max steps
    if (++stepsTaken >= MAX_STEPS) {
        position = startingPosition;
        stepsTaken = 0;
    }
}
