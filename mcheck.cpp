#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Constants for maze elements
enum MazeElements {
    EMPTY = 0, WALL = 1, START = 2, GOAL = 3,
    GOGGLES = 4, SPEED_POTION = 5, FOG = 6, SLOWPOKE_POTION = 7
};

// Directions
enum Direction { NORTH, EAST, SOUTH, WEST };

struct Position {
    int x, y;
};

struct Agent {
    Position position;
    Position previousPosition;
    Direction direction;
    int stepSize;
    int perceptField;
    std::vector<std::string> moveHistory;
    std::vector<int> actionList; // New: List of possible actions
    int lastAction; // New: Last action taken
};


template <typename T>
void printVariable(const T& var) {
    std::cout << var << std::endl;
}


// Function to read maze from a file
std::vector<std::vector<int>> readMaze(const std::string& fileName) {
    std::vector<std::vector<int>> maze;

    std::ifstream file(fileName);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char ch : line) {
            if (ch >= '0' && ch <= '9') {
                row.push_back(ch - '0');
            }
        }
        maze.push_back(row);
    }

    file.close();
    return maze;
}

// Function to print the maze
void printMaze(const std::vector<std::vector<int>>& maze, const Agent& agent) {
    char agentSymbol;
    switch (agent.direction) {
        case NORTH: agentSymbol = '^'; break;
        case EAST:  agentSymbol = '>'; break;
        case SOUTH: agentSymbol = 'V'; break;
        case WEST:  agentSymbol = '<'; break;
    }

    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (agent.position.x == i && agent.position.y == j) {
                std::cout << agentSymbol << " ";  // Display agent with direction
            } else {
                std::cout << maze[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

// Function to initialize the agent
Agent initializeAgent(const std::vector<std::vector<int>>& maze) {
    Agent agent;
    agent.direction = EAST;
    agent.stepSize = 1;
    agent.perceptField = 1;
    agent.actionList = {1, 2, 3}; // Forward, Turn right then forward, Turn left then forward
    agent.lastAction = 0; // No action taken initially
    agent.previousPosition = agent.position;


    // Find the start position
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == START) {
                agent.position = {i, j};
                return agent;
            }
        }
    }

    throw std::runtime_error("Start position not found in the maze.");
}

// Function to move the agent
bool moveAgent(Agent& agent, const std::vector<std::vector<int>>& maze) {
    Position nextPosition = agent.position;
    switch (agent.direction) {
        case NORTH: nextPosition.x -= agent.stepSize; break;
        case EAST:  nextPosition.y += agent.stepSize; break;
        case SOUTH: nextPosition.x += agent.stepSize; break;
        case WEST:  nextPosition.y -= agent.stepSize; break;
    }

    // Check if next position is within maze bounds and not a wall
    if (nextPosition.x >= 0 && nextPosition.x < maze.size() &&
        nextPosition.y >= 0 && nextPosition.y < maze[0].size() &&
        maze[nextPosition.x][nextPosition.y] != WALL) {
        agent.position = nextPosition;
        return true;
    }

    return false;
}

// Function to turn the agent
void turnAgent(Agent& agent, int turnCommand) {
    if (turnCommand == 1) { // Turn left
        agent.direction = static_cast<Direction>((agent.direction + 3) % 4);
    } else if (turnCommand == 3) { // Turn right
        agent.direction = static_cast<Direction>((agent.direction + 1) % 4);
    }
}

// Function to update agent state based on current position
void updateAgentState(Agent& agent, std::vector<std::vector<int>>& maze) {
    int& cell = maze[agent.position.x][agent.position.y];
    switch (cell) {
        case GOGGLES: 
            agent.perceptField = std::min(agent.perceptField + 1, 3);
            cell = EMPTY; // Remove goggles from maze
            break;
        case SPEED_POTION: 
            agent.stepSize = std::min(agent.stepSize + 1, 3);
            cell = EMPTY; // Remove speed potion
            break;
        case FOG: 
            agent.perceptField = std::max(agent.perceptField - 1, 1);
            cell = EMPTY; // Remove fog
            break;
        case SLOWPOKE_POTION: 
            agent.stepSize = std::max(agent.stepSize - 1, 1);
            cell = EMPTY; // Remove slowpoke potion
            break;
    }
}

int decideNextAction(Agent& agent) {
    if (agent.position.x == agent.previousPosition.x && agent.position.y == agent.previousPosition.y) {
        // If the agent hasn't moved, choose a different action
        if (agent.lastAction == 1) { // If last action was forward, try turning
            return (rand() % 2) + 2; // Randomly choose between turning left or right
        } else { // If last action was turning, try forward or turn again
            return (rand() % 2) ? agent.lastAction : 1; // Randomly choose forward or the same turn
        }
    }
    return agent.actionList[rand() % agent.actionList.size()]; // Randomly choose any action
}

int main() {
    std::string fileName = "maze.txt";
    auto maze = readMaze(fileName);
    Agent agent = initializeAgent(maze);
    int steps = 0;



    moveAgent(agent, maze);
    agent.moveHistory.push_back("Forward");
    steps++;

    printMaze(maze, agent);
    

    // Main game loop modification to use RL agent
    // Main game loop modification to use RL agent
    while (true) {
        int action = decideNextAction(agent);
        agent.previousPosition = agent.position;

        if (action == 1) { // Forward
            moveAgent(agent, maze);
            agent.lastAction = 1;
            agent.moveHistory.push_back("Forward");
        } else if (action == 2) { // Turn right then forward
            turnAgent(agent, 3); // Right turn
            moveAgent(agent, maze);
            agent.lastAction = 2;
            agent.moveHistory.push_back("Turn Right, Forward");
        } else if (action == 3) { // Turn left then forward
            turnAgent(agent, 1); // Left turn
            moveAgent(agent, maze);
            agent.lastAction = 3;
            agent.moveHistory.push_back("Turn Left, Forward");
        }

        updateAgentState(agent, maze);
        steps++;
        printMaze(maze, agent);
        std::cout << "-------------------------------------" << std::endl;

        // Check if goal is reached
        if (maze[agent.position.x][agent.position.y] == GOAL) {
            std::cout << "Goal reached in " << steps << " steps!" << std::endl;

            // Print the list of moves
            std::cout << "List of moves: ";
            for (const auto& move : agent.moveHistory) {
                std::cout << move << "; ";
            }
            std::cout << std::endl;

            break;
        }
    }



    return 0;
}






// For manual mode if you want to play
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
