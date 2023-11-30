#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility> // For std::pair
#include <limits>  // For std::numeric_limits
#include <algorithm> // For std::max_element
#include <unordered_map>



// Enumeration representing different elements that can be found in the maze.
enum MazeElements {
    EMPTY = 0, // Represents an empty cell
    WALL = 1, // Represents a wall
    START = 2, // Represents the start position
    GOAL = 3, // Represents the goal position
    GOGGLES = 4, // Represents goggles item
    SPEED_POTION = 5, // Represents a speed potion
    FOG = 6, // Represents fog
    SLOWPOKE_POTION = 7 // Represents a slowpoke potion
};

// Enumeration representing the four cardinal directions.
enum Direction { NORTH, EAST, SOUTH, WEST };

// Structure representing a position in the maze using x and y coordinates.
struct Position {
    int x, y;
};

// Functor for hashing a pair of values. Useful for pairs used as keys in hash maps.
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        auto h1 = std::hash<T1>{}(pair.first);
        auto h2 = std::hash<T2>{}(pair.second);
        return h1 ^ h2; // Combines the hash of the first and second element.
    }
};

// Structure representing an agent navigating the maze.
struct Agent {
    Position position; // Current position of the agent
    Position previousPosition; // Previous position of the agent
    Direction direction; // Current direction of the agent
    int stepSize; // Step size of the agent
    int perceptField; // Perceptual field of the agent
    std::vector<std::string> moveHistory; // History of moves taken by the agent
    std::vector<int> actionList; // List of possible actions the agent can take
    int lastAction; // The last action taken by the agent
    int positionChangeCount; // Counts changes in position
    
    double learningRate; // Learning rate for the Q-learning algorithm
    double discountFactor; // Discount factor for the Q-learning algorithm
    double explorationRate; // Exploration rate for the Q-learning algorithm
    std::unordered_map<std::pair<int, int>, std::vector<double>, pair_hash> QTable; // Q-table for storing state-action values
};

// Template function to print a variable to the console.
template <typename T>
void printVariable(const T& var) {
    std::cout << var << std::endl;
}

// Function to read a maze from a file and return it as a 2D vector of integers.
std::vector<std::vector<int>> readMaze(const std::string& fileName) {
    std::vector<std::vector<int>> maze;

    std::ifstream file(fileName);
    std::string line;

    // Read each line from the file and convert to a vector of integers.
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char ch : line) {
            if (ch >= '0' && ch <= '9') {
                row.push_back(ch - '0'); // Convert character to integer and add to row.
            }
        }
        maze.push_back(row); // Add row to the maze.
    }

    file.close();
    return maze; // Return the constructed maze.
}



// Function to print the maze with the agent's current position and direction.
void printMaze(const std::vector<std::vector<int>>& maze, const Agent& agent) {
    char agentSymbol;
    // Determine the symbol to represent the agent's direction.
    switch (agent.direction) {
        case NORTH: agentSymbol = '^'; break;
        case EAST:  agentSymbol = '>'; break;
        case SOUTH: agentSymbol = 'V'; break;
        case WEST:  agentSymbol = '<'; break;
    }

    // Iterate through the maze and print each cell.
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            // Print the agent symbol if the current cell is the agent's position.
            if (agent.position.x == i && agent.position.y == j) {
                // Validate if the agent's position is within the bounds of the maze.
                if (agent.position.x >= 0 && agent.position.x < maze.size() &&
                    agent.position.y >= 0 && agent.position.y < maze[i].size()) {
                    std::cout << agentSymbol << " "; // Display agent with its direction.
                } else {
                    // Handle out-of-bounds position.
                    std::cout << "X "; // Print 'X' or other symbol for invalid position.
                }
            } else {
                // Print the maze element for non-agent cells.
                std::cout << maze[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

// Function to initialize the agent with initial settings and QTable.
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










int main() {
    // Set the maximum number of steps the agent can take
    int maxSteps = 20000; 
    std::string fileName = "maze.txt";
    // Read the maze from a file
    auto maze = readMaze(fileName);
    // Initialize the agent with its starting position and parameters
    Agent agent = initializeAgent(maze);
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






// // For manual mode if you want to play
// int main() {
//     std::string fileName = "maze.txt";
//     auto maze = readMaze(fileName);
//     Agent agent = initializeAgent(maze);
//     int steps = 0;

//     printMaze(maze, agent);

//     while (true) {
//         int command;
//         std::cout << "Enter command (2=Forward, 1=Left, 3=Right): ";
//         std::cin >> command;

//         if (command == 2) {
//             if (moveAgent(agent, maze)) {
//                 updateAgentState(agent, maze);
//                 steps++;
//             }
//         } else if (command == 1 || command == 3) {
//             turnAgent(agent, command);
//         } else {
//             std::cout << "Invalid command!" << std::endl;
//             continue;
//         }

//         printMaze(maze, agent);
//         std::cout << "-------------------------------------" << std::endl;
//         if (maze[agent.position.x][agent.position.y] == GOAL) {
//             std::cout << "Goal reached in " << steps << " steps!" << std::endl;
//             break;
//         }
//     }

//     return 0;
// }
