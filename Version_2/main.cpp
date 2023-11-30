
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
#include "Agent.h"
#include "MazeUtils.cpp"
#include "MazeUtils.h" // Include the fi le where GOAL is defined
#include "AgentUtils.cpp"


using namespace std;

int main() {
    // Set the maximum number of steps the agent can take
    int maxSteps = 20000; 
    std::string fileName = "maze.txt";
    auto maze = readMaze(fileName);

    // Initialize the agent with its starting position and parameters
    Agent agent = initializeAgent(maze); // Ensure this function returns an Agent type

    int steps = 0;

    // Perform the first move of the agent
    moveAgent(agent, maze);
    agent.moveHistory.push_back("Forward");
    steps++;

    // Print the initial state of the maze with the agent's position
    printMaze(maze, agent);
    
    // Main game loop to iterate through the steps of the agent
    while (steps < maxSteps) {
        // Output the current step and agent's position
        std::cout << "Step " << steps << ": Position (" << agent.position.x << ", " << agent.position.y << ")" << std::endl;

        // Decide the next action for the agent based on its current state and the maze
        int action = decideNextAction(agent, maze);
    
        // Update the agent's previous position
        agent.previousPosition = agent.position;
        
        // Check if the maximum number of steps has been reached
        if (steps >= maxSteps) {
            std::cout << "Maximum steps reached. Exiting." << std::endl;
            break;
        }

        // Check if the agent's current position is within maze boundaries and not a wall
        if (agent.position.x >= 0 && agent.position.x < maze.size() &&
            agent.position.y >= 0 && agent.position.y < maze[agent.position.x].size()) {

            // Check if the agent has reached the goal
            if (maze[agent.position.x][agent.position.y] == GOAL) {
                // Output success message and the list of moves taken
                std::cout << "Goal reached in " << steps << " steps!" << std::endl;
                std::cout << "List of moves: ";
                for (const auto& move : agent.moveHistory) {
                    std::cout << move << "; ";
                }
                std::cout << std::endl;
                std::cout << "Position changed " << agent.positionChangeCount << " times." << std::endl;
                break; // Exit the loop since the goal is reached
            }
        } else {
            // Handle the case where the agent is out of the maze bounds
            std::cerr << "Error: Agent is out of bounds." << std::endl;
            break; // Consider breaking the loop or resetting the agent's position
        }

        // Perform actions based on the chosen action
        if (action == 2) { // Forward
            moveAgent(agent, maze);
            agent.lastAction = 2;
            agent.moveHistory.push_back("Forward");
        } else if (action == 3) { // Turn right then forward
            turnAgent(agent, 3); // Right turn
            moveAgent(agent, maze);
            agent.lastAction = 2;
            agent.moveHistory.push_back("Turn Right, Forward");
        } else if (action == 1) { // Turn left then forward
            turnAgent(agent, 1); // Left turn
            moveAgent(agent, maze);
            agent.lastAction = 1;
            agent.moveHistory.push_back("Turn Left, Forward");
        }

        // Update the agent's state based on its new position
        updateAgentState(agent, maze);
        steps++;
        // Print the maze after each move
        printMaze(maze, agent);
        std::cout << "-------------------------------------" << std::endl;

        // Update Q-values based on the agent's actions and rewards
        std::pair<int, int> oldState = {agent.previousPosition.x, agent.previousPosition.y};
        std::pair<int, int> newState = {agent.position.x, agent.position.y};
        int actionIndex = std::find(agent.actionList.begin(), agent.actionList.end(), action) - agent.actionList.begin();

        // Calculate reward based on the current position in the maze
        double reward = (maze[newState.first][newState.second] == GOAL) ? 100.0 : -1.0;

        // Correct way to find the maximum Q-value for the new state
        auto maxElementIter = std::max_element(agent.QTable[newState].begin(), agent.QTable[newState].end());
        double maxQValue = (maxElementIter != agent.QTable[newState].end()) ? *maxElementIter : 0.0;

        // Update the Q-table
        agent.QTable[oldState][actionIndex] += agent.learningRate * (reward + agent.discountFactor * maxQValue - agent.QTable[oldState][actionIndex]);

        // Check if goal is reached
        if (maze[agent.position.x][agent.position.y] == GOAL) {
            std::cout << "Goal reached in " << steps << " steps!" << std::endl;

            // Print the list of moves
            std::cout << "List of moves: ";
            for (const auto& move : agent.moveHistory) {
                std::cout << move << "; ";
            }
            std::cout << std::endl;
            std::cout << "Goal reached in " << steps << " steps!" << std::endl;
            std::cout << "Position changed " << agent.positionChangeCount << " times." << std::endl;

            break;
        }
        if (++steps >= maxSteps) {
            std::cerr << "Maximum steps reached. Exiting loop." << std::endl;
            break;
    }

    
    }

    return 0;
}


