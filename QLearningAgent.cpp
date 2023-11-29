#include "QLearningAgent.h"
#include <utility> // For std::pair
#include <cstdlib> // For std::rand, std::srand
#include <ctime> // For std::time
#include <algorithm> // For std::max_element
#include "Agent.h"

// Assuming these are constant for the maze
const int GOAL_ROW = 12; // 21st row in 0-based indexing
const int GOAL_COL = 6; // 21st column in 0-based indexing


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
    std::vector<int> possibleActions = {0, 1, 2, 3};  // Up, Right, Down, Left

    // Remove the opposite of the last action
    if (lastAction != -1) {
        possibleActions.erase(std::remove(possibleActions.begin(), possibleActions.end(), (lastAction + 2) % 4), possibleActions.end());
    }

    // Epsilon-greedy strategy
    if ((std::rand() % 100) / 100.0 < EPSILON) {
        // Choose a random action from the remaining possible actions
        lastAction = possibleActions[std::rand() % possibleActions.size()];
    } else {
        // Choose the best action based on Q-values from the remaining possible actions
        // ...
        // Update lastAction with the chosen action
    }

    return lastAction;
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
    std::pair<int, int> newPosition = getNextPosition(maze, position, action, lastAction);  // Pass maze and lastAction
    // ... rest of the method ...
    // Calculate new position based on action
    
    int calcRow = position.first;
    int calcCol = position.second;
    // std::cout << "Attempting calc to: (" << calcRow << ", " << calcCol << ")" << std::endl;
    // Check for boundaries and walls
    if (isValidMove(maze, newPosition)) {
        // Update position if valid move
        position = newPosition;
        setPosition(newPosition.first, newPosition.second);
    }

    int reward = calculateReward(maze, position.first, position.second);
    updateQValues(action, reward, position.first, position.second);

    // Check for max steps
    if (++stepsTaken >= MAX_STEPS) {
        reset(); // Reset agent position and step count
    }
}

std::pair<int, int> QLearningAgent::getNextPosition(const Maze &maze, std::pair<int, int> currentPosition, int action, int lastAction) {
    int newRow = currentPosition.first;
    int newCol = currentPosition.second;
    std::pair<int, int> alternativePosition;

    // If the chosen action is a reversal of the last action, try other directions
    if ((action == 0 && lastAction == 2) || // Up after Down
        (action == 1 && lastAction == 3) || // Right after Left
        (action == 2 && lastAction == 0) || // Down after Up
        (action == 3 && lastAction == 1)) { // Left after Right

        std::vector<int> alternativeActions = {0, 1, 2, 3}; // All possible actions
        alternativeActions.erase(std::remove(alternativeActions.begin(), alternativeActions.end(), action), alternativeActions.end()); // Remove chosen action
        alternativeActions.erase(std::remove(alternativeActions.begin(), alternativeActions.end(), (lastAction + 2) % 4), alternativeActions.end()); // Remove direct reversal

        // Try alternative actions
        for (int altAction : alternativeActions) {
            alternativePosition = calculateNewPosition(currentPosition, altAction);
            // Check if the alternative position is valid
            if (isValidMove(maze, alternativePosition)) {
                return alternativePosition;
            }
        }

        // If no valid alternative move, return the last action's position
        return calculateNewPosition(currentPosition, lastAction);
    }

    // Normal action calculation
    return calculateNewPosition(currentPosition, action);
}

// Helper function to calculate new position based on an action
std::pair<int, int> QLearningAgent::calculateNewPosition(std::pair<int, int> currentPosition, int action) {
    int newRow = currentPosition.first;
    int newCol = currentPosition.second;

    switch (action) {
        case 1: newRow--; break; // Up
        case 0: newCol++; break; // Right
        case 3: newRow++; break; // Down
        case 2: newCol--; break; // Left
    }
    return std::make_pair(newRow, newCol);
}



bool QLearningAgent::isValidMove(const Maze &maze, std::pair<int, int> newPosition) {
    // Get the size of the maze and check if the new position is within bounds and not a wall
    std::pair<int, int> mazeSize = maze.getSize();
    int mazeRows = 21;
    int mazeCols = 21;
    int newRow = newPosition.first;
    int newCol = newPosition.second;

    // Debugging: Print current and new position
    std::cout << "Current Position: (" << position.first << ", " << position.second << ")" << std::endl;
    std::cout << "Attempting Move to: (" << newRow << ", " << newCol << ")" << std::endl;

    // Check if the new position is within the maze boundaries
    if (newRow < 0 || newRow >= mazeRows || newCol < 0 || newCol >= mazeCols) {
        // Debugging: Print out of bounds message
        std::cout << "Move Invalid: New position out of bounds.("<< newRow<< ","<< mazeRows << "," << newCol<< "," << mazeCols<< ")" << std::endl;
        return false;
    }

    // Check if the new position is not a wall
    int cellValue = maze.at(newRow, newCol);
    if (cellValue == 1) {  // Assuming 1 represents wall cells
        // Debugging: Print wall collision message along with the cell value
        // std::cout << "Move Invalid: New position (" << newRow << ", " << newCol 
        //         << ") is a wall. Cell Value: " << cellValue << std::endl;
        return false;
}


    // Debugging: Print valid move message
    // std::cout << "Move Valid: New position is free." << std::endl;
    return true;
}

std::pair<int, int> findStartingPosition(const std::vector<std::vector<int>>& mazeGrid) {
    for (int row = 0; row < mazeGrid.size(); ++row) {
        for (int col = 0; col < mazeGrid[row].size(); ++col) {
            if (mazeGrid[row][col] == 2) {  // 2 represents the starting position
                return std::make_pair(row, col);
            }
        }
    }
    return std::make_pair(0, 0); // Default starting position if '2' is not found
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