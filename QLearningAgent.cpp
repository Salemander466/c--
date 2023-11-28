#include "QLearningAgent.h"
#include <utility> // For std::pair
#include <cstdlib> // For std::rand, std::srand
#include <ctime> // For std::time
#include <algorithm> // For std::max_element

// Assuming these are constant for the maze
const int GOAL_ROW = 20; // 21st row in 0-based indexing
const int GOAL_COL = 20; // 21st column in 0-based indexing


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

int QLearningAgent::calculateReward(const Maze &maze, int row, int col) {
    int cellValue = maze.at(row, col);

    switch (cellValue) {
        case 0: // Empty space
            return STEP_REWARD;
        case 1: // Wall
            return WALL_PENALTY;
        case 2: // Goggles
            return GOGGLES_REWARD;
        case 3: // Speed potion
            return SPEED_POTION_REWARD;
        case 4: // Fog
            return FOG_PENALTY;
        case 5: // Slowpoke potion
            return SLOWPOKE_POTION_PENALTY;
        case 6: // Goal
            return GOAL_REWARD;
        default:
            return 0; // Default case, if needed
    }
}

void QLearningAgent::move(const Maze &maze) {
    int action = chooseAction(maze); // Choose action based on Q-values and epsilon-greedy strategy
    
    // Calculate new position based on action
    std::pair<int, int> newPosition = getNextPosition(position, action);

    // Check for boundaries and walls
    if (isValidMove(maze, newPosition)) {
        // Update position if valid move
        position = newPosition;
    }

    int reward = calculateReward(maze, position.first, position.second);
    updateQValues(action, reward, position.first, position.second);

    // Check for max steps
    if (++stepsTaken >= MAX_STEPS) {
        reset(); // Reset agent position and step count
    }
}

std::pair<int, int> QLearningAgent::getNextPosition(std::pair<int, int> currentPosition, int action) {
    int newRow = currentPosition.first;
    int newCol = currentPosition.second;

    // Assuming action encoding: 0 up, 1 right, 2 down, 3 left
    switch (action) {
        case 0: newRow--; break;
        case 1: newCol++; break;
        case 2: newRow++; break;
        case 3: newCol--; break;
    }
    return std::make_pair(newRow, newCol);
}

bool QLearningAgent::isValidMove(const Maze &maze, std::pair<int, int> newPosition) {
    // Get the size of the maze and check if the new position is within bounds and not a wall
    std::pair<int, int> mazeSize = maze.getSize();
    int mazeRows = mazeSize.first;
    int mazeCols = mazeSize.second;
    int newRow = newPosition.first;
    int newCol = newPosition.second;

    // Check if the new position is within the maze boundaries and not a wall
    if (newRow < 0 || newRow >= mazeRows || newCol < 0 || newCol >= mazeCols) {
        // Out of bounds
        return false;
    }

    // Check if the new position is a wall
    if (maze.at(newRow, newCol) == 1) {
        // Position is a wall
        return false;
    }

    // The move is valid
    return true;
}



// Implementation of hasReachedGoal
bool QLearningAgent::hasReachedGoal(const Maze &maze) {
    return position.first == GOAL_ROW && position.second == GOAL_COL;
}

// Implementation of reset
void QLearningAgent::reset() {
    position = startingPosition;
    stepsTaken = 0;
}