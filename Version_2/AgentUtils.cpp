#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility> // For std::pair
#include <limits>  // For std::numeric_limits
#include <algorithm> // For std::max_element
#include <unordered_map>

#include "AgentUtils.h"

enum MazeElements {
    EMPTY = 0, WALL = 1, START = 2, GOAL = 3,
    GOGGLES = 4, SPEED_POTION = 5, FOG = 6, SLOWPOKE_POTION = 7
};


Agent initializeAgent(const std::vector<std::vector<int>>& maze) {
    Agent agent;

    // Set initial properties for the agent.
    agent.direction = EAST; // Initial direction.
    agent.stepSize = 1; // Step size for the agent's movement.
    agent.perceptField = 1; // Perceptual field (range of sensing).
    agent.actionList = {1, 2, 3}; // Define possible actions (example: forward, turn right, turn left).
    agent.lastAction = 0; // Initialize with no action taken.
    agent.previousPosition = agent.position; // Set initial previous position.
    agent.positionChangeCount = 0; // Initialize position change count.
    agent.learningRate = 0.1; // Learning rate for Q-learning.
    agent.discountFactor = 0.9; // Discount factor for Q-learning.
    agent.explorationRate = 0.5; // Exploration rate for Q-learning.

    // Initialize Q-table with zero values for each state-action pair.
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            agent.QTable[{i, j}] = std::vector<double>(agent.actionList.size(), 0.0);
        }
    }

    // Find and set the initial position of the agent based on the START position in the maze.
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == START) {
                agent.position = {i, j}; // Set position to START location.
                return agent; // Return the initialized agent.
            }
        }
    }

    // Handle case where START position is not found in the maze.
    std::cerr << "Error: START position not found in the maze. Setting default position (0,0)." << std::endl;
    agent.position = {0, 0}; // Set a default starting position.
    return agent; // Return the agent with default position.
}

    


// Function to move the agent in the direction it is facing.
bool moveAgent(Agent& agent, const std::vector<std::vector<int>>& maze) {
    Position nextPosition = agent.position; // Calculate the next position based on current position.

    // Adjust the next position based on the agent's direction and step size.
    switch (agent.direction) {
        case NORTH: nextPosition.x -= agent.stepSize; break;
        case EAST:  nextPosition.y += agent.stepSize; break;
        case SOUTH: nextPosition.x += agent.stepSize; break;
        case WEST:  nextPosition.y -= agent.stepSize; break;
    }

    // Check if the next position is valid (within bounds and not a wall).
    if (nextPosition.x >= 0 && nextPosition.x < maze.size() &&
        nextPosition.y >= 0 && nextPosition.y < maze[0].size() &&
        maze[nextPosition.x][nextPosition.y] != WALL) {
        
        agent.position = nextPosition; // Update the agent's position.
        return true; // Move was successful.
    } else {
        return false; // Move was invalid, agent did not move.
    }
}


// Function to turn the agent based on a turn command.
void turnAgent(Agent& agent, int turnCommand) {
    // Adjust the agent's direction based on the turn command.
    if (turnCommand == 1) { // Turn left
        agent.direction = static_cast<Direction>((agent.direction + 3) % 4);
    } else if (turnCommand == 3) { // Turn right
        agent.direction = static_cast<Direction>((agent.direction + 1) % 4);
    }
    // Note: No action for 'turnCommand == 2' which could be turn around, etc.
}

// Function to update the agent's state based on its current position in the maze.
void updateAgentState(Agent& agent, std::vector<std::vector<int>>& maze) {
    // Ensure the agent's position is within the maze boundaries.
    if (agent.position.x >= 0 && agent.position.x < maze.size() &&
        agent.position.y >= 0 && agent.position.y < maze[agent.position.x].size()) {
        
        int& cell = maze[agent.position.x][agent.position.y]; // Reference to the cell at the agent's position.

        // Process the cell based on its value and update the agent's state accordingly.
        switch (cell) {
            case GOGGLES: 
                agent.perceptField = std::min(agent.perceptField + 1, 3); // Increase perceptual field.
                cell = EMPTY; // Remove goggles from the maze.
                break;
            case SPEED_POTION: 
                agent.stepSize = std::min(agent.stepSize + 1, 3); // Increase movement speed.
                cell = EMPTY; // Remove speed potion.
                break;
            case FOG: 
                agent.perceptField = std::max(agent.perceptField - 1, 1); // Decrease perceptual field.
                cell = EMPTY; // Remove fog.
                break;
            case SLOWPOKE_POTION: 
                agent.stepSize = std::max(agent.stepSize - 1, 1); // Decrease movement speed.
                cell = EMPTY; // Remove slowpoke potion.
                break;
        }
    }
    else {
        // Handle cases where the agent's position is outside the maze boundaries.
        std::cerr << "Error: Agent is out of bounds." << std::endl;
        // Additional logic for out-of-bounds cases can be added here.
    }
}



int decideNextAction(Agent& agent, const std::vector<std::vector<int>>& maze) {
    // Current state of the agent in the maze represented as a pair (x, y).
    std::pair<int, int> currentState = {agent.position.x, agent.position.y};

    // Restrict the agent's possible actions to: 
    // 1 - Turn Left then Forward, 2 - Forward, 3 - Turn Right then Forward
    agent.actionList = {1, 2, 3};

    // Initialize the best action and set a very low initial max Q-Value.
    int bestAction = 2; // Default action, could be any valid action.
    double maxQValue = -std::numeric_limits<double>::infinity(); // Lowest possible double value.

    // Decide whether to explore (try new actions) or exploit (use best-known actions).
    if ((double)rand() / RAND_MAX < agent.explorationRate) {
        // Exploration: Randomly choose an action from the available action list.
        return agent.actionList[rand() % agent.actionList.size()];
    } else {
        // Exploitation: Choose the action with the highest Q-value from the QTable.
        for (size_t i = 0; i < agent.actionList.size(); ++i) {
            int action = agent.actionList[i]; // Get an action from the list.
            double qValue = agent.QTable[currentState][action]; // Retrieve Q-value from the QTable.

            // If the Q-value of this action is higher than the current max, update the best action.
            if (qValue > maxQValue) {
                maxQValue = qValue;
                bestAction = action;
            }
        }
        return bestAction; // Return the best action based on the highest Q-value.
    }
}