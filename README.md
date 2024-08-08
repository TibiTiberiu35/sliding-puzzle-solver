# SlidingPuzzleSolver

## Overview

The SlidingPuzzleSolver is a C++ project designed to solve sliding puzzles, such as the 4x4 puzzle where the goal is to arrange the tiles in a specific order with the empty space denoted by `0`. This solver reads puzzle configurations from `input.txt` and outputs the steps required to solve the puzzle in `output.txt`. Some complex puzzles might not be solvable within practical time or memory limits.

## How It Works

### A\* Algorithm

The SlidingPuzzleSolver utilizes the A\* (A-star) search algorithm, which is a popular and efficient pathfinding and graph traversal algorithm. Here's a detailed explanation of how it works in this project:

1. **Initialization**:

   - The algorithm starts with the initial configuration of the puzzle read from `input.txt`.

2. **Priority Queue**:

   - A priority queue is used to explore the most promising paths first. The priority is determined by a cost function `f(n) = g(n) + h(n)`, where:
     - `g(n)` is the cost to reach the current state from the initial state (i.e., the number of moves taken so far).
     - `h(n)` is the heuristic estimate of the cost to reach the goal state from the current state. In this case, the heuristic is the sum of the Manhattan distances of the tiles from their goal positions.

3. **Expansion**:

   - The algorithm expands the most promising node by generating its neighboring nodes (possible moves in the puzzle). The new Manhattan distance is calculated for each neighboring state.

4. **Goal Check**:

   - If the current node represents the goal state (Manhattan distance is 0), the algorithm terminates and returns the solution path.

5. **Loop**:
   - This process continues until the goal state is found or there are no more nodes to explore.

### Input and Output

- **Input**: The puzzle configuration should be provided in `input.txt`, where the empty space is denoted by `0`.
- **Output**: The steps to solve the puzzle will be written to `output.txt`.

## Usage

### Prerequisites

- Visual Studio (preferably the latest version)
- C++ compiler

### Setup and Build

1. **Clone the Repository**

   ```sh
   git clone https://github.com/tbtiberiu/SlidingPuzzleSolver.git
   cd SlidingPuzzleSolver
   ```

2. **Open in Visual Studio**

   - Launch Visual Studio.
   - Open the project by navigating to `File > Open > Project/Solution` and select the `SlidingPuzzleSolver.sln` file.

3. **Build the Project**

   - In the Solution Explorer, right-click on the `SlidingPuzzleSolver` project and select `Build`.

4. **Run the Project**
   - Ensure `input.txt` is in the same directory as the executable or provide the path to the file in the code.
   - Run the project by pressing `F5` or selecting `Debug > Start Debugging`.

### Example

**Input (`input.txt`):**

```
3
3 5 2
1 0 4
8 6 7
```

**Output (`output.txt`):**

```
Move 0 (START):
3 5 2
1   4
8 6 7

Move 1 (DOWN):
Moved space from (1, 1) to (2, 1).
3 5 2
1 6 4
8   7

...

Move 22 (DOWN):
Moved space from (1, 2) to (2, 2).
1 2 3
4 5 6
7 8
```

## Code Overview

The main components of the solver are as follows:

**State Struct**: Represents the state of the puzzle, including the size, body (tiles), and the coordinates of the empty space.

**Node Struct**: Represents a node in the search tree, including the state, level (number of moves from the initial state), Manhattan sum (heuristic), move (the move that led to this state), and a pointer to the parent node.

**Functions**:

- `clearFile`: Clears the output file.
- `readState`: Reads the initial puzzle state from the input file.
- `writeState`: Writes the current state to the output file.
- `printList`: Prints the sequence of moves to solve the puzzle.
- `getInitialManhattanDistance`: Computes the initial Manhattan distance for the given state.
- `isSolvable`: Checks if the puzzle is solvable.
- `generateNewStates`: Generates new states from the current state and pushes them onto the priority queue.

## Troubleshooting

### Common Issues

**Memory or Time Limits**: Some puzzles might be too complex to solve within practical limits. Try a simpler configuration.

**Compilation Errors**: Ensure all dependencies are correctly configured in Visual Studio. This project uses the ISO C++20 Language Standard.
