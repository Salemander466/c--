#ifndef AGENTUTILS_H
#define AGENTUTILS_H

#include <vector>
#include <string>
#include <map>
#include "Agent.h"  // Assuming Agent.h contains the definition of the Agent struct and related enums.

// Function to initialize the agent with initial settings and QTable.
Agent initializeAgent(const std::vector<std::vector<int>>& maze);

// Function to move the agent in the direction it is facing.
bool moveAgent(Agent& agent, const std::vector<std::vector<int>>& maze);

// Function to turn the agent based on a turn command.
void turnAgent(Agent& agent, int turnCommand);

// Function to update the agent's state based on its current position in the maze.
void updateAgentState(Agent& agent, std::vector<std::vector<int>>& maze);

// Function to decide the next action for the agent based on its current state and the maze.
int decideNextAction(Agent& agent, const std::vector<std::vector<int>>& maze);

#endif // AGENTUTILS_H
