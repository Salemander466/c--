#include <iostream>
#include "Maze.h"
#include "Maze.cpp"
#include "Agent.h"
#include "QLearningAgent.h"
#include "QLearningAgent.cpp"

// #include "Maze.txt"

int main() {
    Maze maze("maze.txt"); // Create a Maze object, loading the maze from the file
    QLearningAgent agent(0, 0); // Initialize the agent at start position (1, 1)
    // maze.loadMaze("maze.txt");
    // maze.printMaze();
    // Training loop
    int numberToFind = 6; 
    const int MAX_STEPS_PER_EPISODE = 5000;
    std::pair<int, int> coordinates = maze.findNumberCoordinates(numberToFind);
    if (coordinates.first != -1) {
        std::cout << "Number " << numberToFind << " found at (" 
                  << coordinates.first << ", " << coordinates.second << ")" << std::endl;
    } else {
        std::cout << "Number " << numberToFind << " not found in the maze." << std::endl;
    }



    for (int episode = 0; episode < 1000; episode++) {
        int stepCount = 0; // To keep track of the number of steps in this episode
        if (stepCount == 20){
            break;
        }
        while (!agent.hasReachedGoal(maze) && stepCount < MAX_STEPS_PER_EPISODE) {
            agent.move(maze);
            stepCount++;

            // if (stepCount % 100 == 0) {  // Print every 1000 steps
            std::pair<int, int> position = agent.getPosition(); // Get the position from the agent
            int row = position.first;  // Extract the row
            int col = position.second; // Extract the column

            std::cout << "Episode: " << episode 
                        << ", Step: " << stepCount 
                        << ", Position: (" << row << ", " << col << ")" << std::endl;
            // }
        }

        std::cout << "Episode " << episode << " completed in " << stepCount << " steps." << std::endl;
        agent.reset(); // Reset the agent to the starting position for the next episode
    }

    // Demonstration of the agent's learned behavior
    std::cout << "Demonstrating learned behavior:\n";
    agent.setPosition(0, 0); // Reset the agent to the starting position
    while (!agent.hasReachedGoal(maze)) {
        agent.move(maze);
        // displayMaze(maze, agent); // Uncomment this if you have a displayMaze function
    }

    return 0;
}
