#include <iostream>
#include "Maze.h"
#include "QLearningAgent.h"

int main() {
    Maze maze("maze.txt"); // Create a Maze object, loading the maze from the file
    QLearningAgent agent(1, 1); // Initialize the agent at start position (1, 1)

    // Training loop
    for (int episode = 0; episode < 1000; episode++) {
        while (!agent.hasReachedGoal(maze)) { // Assuming hasReachedGoal takes Maze as parameter
            agent.move(maze);
        }
        agent.reset(); // Reset the agent to the starting position for the next episode
    }

    // Demonstration of the agent's learned behavior
    std::cout << "Demonstrating learned behavior:\n";
    agent.setPosition(1, 1); // Reset the agent to the starting position
    while (!agent.hasReachedGoal(maze)) { // Assuming hasReachedGoal takes Maze as parameter
        agent.move(maze);
        // displayMaze(maze, agent); // Uncomment this if you have a displayMaze function
    }

    return 0;
}
