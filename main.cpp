#include <iostream>
#include "Maze.h"
#include "Maze.cpp"
#include "Agent.h"
#include "QLearningAgent.h"
#include "QLearningAgent.cpp"

// #include "Maze.txt"

int main() {
    Maze maze("C:/Users/teams/Documents/CS&AI year 3/cpp_reinforcement_learning_maze_solver/maze.txt"); // Create a Maze object, loading the maze from the file
    QLearningAgent agent(1, 1); // Initialize the agent at start position (1, 1)

    // Training loop
    for (int episode = 0; episode < 1000; episode++) {
        int stepCount = 0; // To keep track of the number of steps in this episode

        while (!agent.hasReachedGoal(maze)) {
            agent.move(maze);
            stepCount++;

            if (stepCount % 1000 == 0) {  // Print every 1000 steps
                std::pair<int, int> position = agent.getPosition(); // Get the position from the agent
                int row = position.first;  // Extract the row
                int col = position.second; // Extract the column

                std::cout << "Episode: " << episode 
                          << ", Step: " << stepCount 
                          << ", Position: (" << row << ", " << col << ")" << std::endl;
            }
        }

        std::cout << "Episode " << episode << " completed in " << stepCount << " steps." << std::endl;
        agent.reset(); // Reset the agent to the starting position for the next episode
    }

    // Demonstration of the agent's learned behavior
    std::cout << "Demonstrating learned behavior:\n";
    agent.setPosition(1, 1); // Reset the agent to the starting position
    while (!agent.hasReachedGoal(maze)) {
        agent.move(maze);
        // displayMaze(maze, agent); // Uncomment this if you have a displayMaze function
    }

    return 0;
}
