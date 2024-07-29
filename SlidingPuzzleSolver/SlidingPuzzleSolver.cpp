#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <queue>
#include <set>

struct Directions
{
    std::string move;
    short x = 0, y = 0;
};

struct Coordinates
{
    short x = 0, y = 0;
};

struct State
{
    short size;
    std::vector<std::vector<short>> body;
    Coordinates space;
};

struct Node
{
    State state;
    int level = 0;
    int manhattanSum = 0;
    std::string move = "START";
    Node* parent = NULL;

    int getNewManhattanDistance(short dx, short dy)
    {
        int manSum = manhattanSum;
        short x1 = state.space.x, y1 = state.space.y;
        short nr = state.body[x1 + dx][y1 + dy];
        short x2 = (nr - 1) / state.size, y2 = (nr - 1) % state.size;
        short oldManhattanDist = abs(x1 + dx - x2) + abs(y1 + dy - y2);
        short newManhattanDist = abs(x1 - x2) + abs(y1 - y2);
        manSum -= (dx + dy);
        manSum += (oldManhattanDist > newManhattanDist) ? -1 : 1;
        return manSum;
    }
};

struct CompareDist
{
    bool operator()(Node* node1, Node* node2) const
    {
        return node1->level + node1->manhattanSum > node2->level + node2->manhattanSum;
    }
};

void clearFile(std::string fileName);
State readState(std::string fileName);
void writeState(State state, std::string fileName);
void printList(Node* node, std::string fileName);
int getInitialManhattanDistance(State state);
bool isSolvable(State state);
void generateNewStates(Node* current, std::priority_queue<Node*, std::vector<Node*>, CompareDist>& nodesQueue, std::set<std::vector<std::vector<short>>>& oldStates);

int main()
{
    std::string inputFile = "input.txt";
    std::string outputFile = "output.txt";
    State initialState = readState(inputFile);
    clearFile(outputFile);

    if (!isSolvable(initialState))
    {
        std::cout << "This puzzle can not be solved!" << std::endl;
        return 1;
    }

    std::priority_queue<Node*, std::vector<Node*>, CompareDist> nodesQueue;
    std::set<std::vector<std::vector<short>>> oldStates;
    oldStates.insert(initialState.body);
    Node* root = new Node();
    root->state = initialState;
    root->manhattanSum = getInitialManhattanDistance(root->state);
    nodesQueue.push(root);

    while (!nodesQueue.empty())
    {
        Node* currentNode = nodesQueue.top();
        nodesQueue.pop();

        if (currentNode->manhattanSum == 0)
        {
            printList(currentNode, outputFile);
            break;
        }

        generateNewStates(currentNode, nodesQueue, oldStates);
    }

    return 0;
}

void clearFile(std::string fileName)
{
    std::ofstream fout(fileName);
    fout.close();
}

bool isInvalidState(State state, short i, short j, short x)
{
    return x < 0 || x > pow(state.size, 2) - 1 || (j > 0 && state.body[i][j - 1] == x);
}

State readState(std::string fileName)
{
    State state;
    std::ifstream fin(fileName);

    fin >> state.size;

    state.body.resize(state.size);
    for (short i = 0; i < state.size; i++)
    {
        for (short j = 0; j < state.size; j++)
        {
            short x;
            fin >> x;

            if (isInvalidState(state, i, j, x))
            {
                std::cout << "Error: Entered state is invalid!" << std::endl;
                exit(1);
            }

            if (x == 0)
                state.space = { .x = i, .y = j };
            state.body[i].push_back(x);
        }
    }

    fin.close();

    return state;
}

void printList(Node* node, std::string fileName)
{
    if (node == NULL)
        return;
    printList(node->parent, fileName);
    std::ofstream fout(fileName, std::ios_base::app);
    fout << "Move " << node->level << " (" << node->move << "): \n";
    if (node->parent != NULL)
    {
        Coordinates prev = node->parent->state.space;
        Coordinates space = node->state.space;
        fout << "Moved space from (" << prev.x << ", " << prev.y << ')';
        fout << " to (" << space.x << ", " << space.y << ")." << '\n';
    }
    fout.close();
    writeState(node->state, fileName);
}

void writeState(State state, std::string fileName)
{
    std::ofstream fout(fileName, std::ios_base::app);
    for (short i = 0; i < state.size; i++)
    {
        for (short j = 0; j < state.size; j++)
        {
            short x = state.body[i][j];
            if (x == 0)
                fout << ' ';
            else
                fout << x;
            fout << ' ';
        }
        fout << '\n';
    }
    fout << std::endl;
    fout.close();
}

bool isSolvable(State state)
{
    std::vector<short> tiles;
    int inversions = 0;

    for (short i = 0; i < state.size; i++)
        for (short j = 0; j < state.size; j++)
            tiles.push_back(state.body[i][j]);

    for (short i = 0; i < tiles.size() - 1; i++)
        for (short j = i + 1; j < tiles.size(); j++)
            if (tiles[j] && tiles[i] > tiles[j])
                inversions++;

    if (state.size % 2 == 1)
        return (inversions % 2 == 0);
    else
        return (inversions % 2 != state.space.x % 2);
}

int getInitialManhattanDistance(State state)
{
    int manhattanSum = 0;

    for (short i = 0; i < state.size; i++)
        for (short j = 0; j < state.size; j++)
        {
            short x = state.body[i][j];
            short i2 = (x - 1) / state.size, j2 = (x - 1) % state.size;
            if (x == 0)
                i2 = state.size - 1, j2 = state.size - 1;

            short distance = abs(i - i2) + abs(j - j2);
            manhattanSum += distance;
        }

    return manhattanSum;
}

void generateNewStates(Node* current, std::priority_queue<Node*, std::vector<Node*>, CompareDist>& nodesQueue, std::set<std::vector<std::vector<short>>>& oldStates)
{
    Directions d[] = { {.move = "UP", .x = -1}, {.move = "RIGHT", .y = 1}, {.move = "DOWN", .x = 1}, {.move = "LEFT", .y = -1} };
    short x = current->state.space.x, y = current->state.space.y;
    short size = current->state.size;

    for (short i = 0; i < 4; i++)
    {
        Node* newNode = new Node();
        newNode->state = current->state;

        if (x + d[i].x < 0 || y + d[i].y < 0 || x + d[i].x > size - 1 || y + d[i].y > size - 1)
            continue;

        std::swap(newNode->state.body[x][y], newNode->state.body[x + d[i].x][y + d[i].y]);
        newNode->state.space.x += d[i].x;
        newNode->state.space.y += d[i].y;
        newNode->move = d[i].move;

        if (oldStates.find(newNode->state.body) != oldStates.end())
            continue;

        newNode->level = current->level + 1;
        newNode->manhattanSum = current->getNewManhattanDistance(d[i].x, d[i].y);
        newNode->parent = current;

        nodesQueue.push(newNode);
        oldStates.insert(newNode->state.body);
    }
}
