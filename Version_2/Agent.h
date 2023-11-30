#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <string>
#include <map>
#include <utility>
#include <unordered_map>

// Agent struct and related enums here
struct Position {
    int x, y;
};
enum Direction { NORTH, EAST, SOUTH, WEST };

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


#endif
