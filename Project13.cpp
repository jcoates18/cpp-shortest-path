/*
 * Project 13 - Unweighted Shortest Path
 *
 * Name: Jourdyn Coates
 * Date: July 31, 2026
 */
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

class GraphNode
{
private:
    int pointNum;
    int topNum;
    int dist;
    GraphNode* path;
    vector<GraphNode*> adjacentNodes;

public:
    GraphNode(int number)
    {
        pointNum = number;
        topNum = 0;
        dist = numeric_limits<int>::max();
        path = nullptr;
    }

    int getPointNum() const
    {
        return pointNum;
    }

    int getTopNum() const
    {
        return topNum;
    }

    void setTopNum(int number)
    {
        topNum = number;
    }

    int getDist() const
    {
        return dist;
    }

    void setDist(int distance)
    {
        dist = distance;
    }

    GraphNode* getPath() const
    {
        return path;
    }

    void setPath(GraphNode* previousNode)
    {
        path = previousNode;
    }

    void addAdjacentNode(GraphNode* node)
    {
        adjacentNodes.push_back(node);
    }

    const vector<GraphNode*>& getAdjacentNodes() const
    {
        return adjacentNodes;
    }
};

class Graph
{
private:
    vector<GraphNode*> nodes;

    GraphNode* findNode(int pointNum) const
    {
        for (GraphNode* node : nodes)
        {
            if (node->getPointNum() == pointNum)
            {
                return node;
            }
        }

        return nullptr;
    }

    void resetShortestPathData()
    {
        for (GraphNode* node : nodes)
        {
            node->setDist(numeric_limits<int>::max());
            node->setPath(nullptr);
        }
    }

public:
    Graph(const vector<vector<int>>& connectivityMatrix)
    {
        int numberOfNodes = static_cast<int>(connectivityMatrix.size());

        for (int i = 0; i < numberOfNodes; i++)
        {
            nodes.push_back(new GraphNode(i + 1));
        }

        for (int row = 0; row < numberOfNodes; row++)
        {
            for (int column = 0; column < numberOfNodes; column++)
            {
                if (connectivityMatrix[row][column] == 1)
                {
                    nodes[row]->addAdjacentNode(nodes[column]);
                }
            }
        }
    }

    ~Graph()
    {
        for (GraphNode* node : nodes)
        {
            delete node;
        }
    }

    void displayGraph() const
    {
        cout << "\nGraph adjacency list:\n";

        for (GraphNode* node : nodes)
        {
            cout << "Point " << node->getPointNum() << ": ";

            if (node->getAdjacentNodes().empty())
            {
                cout << "No outgoing connections";
            }
            else
            {
                for (GraphNode* adjacentNode : node->getAdjacentNodes())
                {
                    cout << adjacentNode->getPointNum() << " ";
                }
            }

            cout << endl;
        }
    }

    void topsort()
    {
        int numberOfNodes = static_cast<int>(nodes.size());
        vector<int> indegree(numberOfNodes, 0);
        queue<GraphNode*> nodeQueue;

        for (GraphNode* node : nodes)
        {
            node->setTopNum(0);

            for (GraphNode* adjacentNode : node->getAdjacentNodes())
            {
                indegree[adjacentNode->getPointNum() - 1]++;
            }
        }

        for (GraphNode* node : nodes)
        {
            if (indegree[node->getPointNum() - 1] == 0)
            {
                nodeQueue.push(node);
            }
        }

        int counter = 0;

        while (!nodeQueue.empty())
        {
            GraphNode* currentNode = nodeQueue.front();
            nodeQueue.pop();

            currentNode->setTopNum(++counter);

            for (GraphNode* adjacentNode : currentNode->getAdjacentNodes())
            {
                indegree[adjacentNode->getPointNum() - 1]--;

                if (indegree[adjacentNode->getPointNum() - 1] == 0)
                {
                    nodeQueue.push(adjacentNode);
                }
            }
        }

        if (counter != numberOfNodes)
        {
            cout << "\nThe graph contains a cycle, so a topological ordering cannot be completed.\n";
        }
        else
        {
            cout << "\nTopological numbering:\n";

            for (GraphNode* node : nodes)
            {
                cout << "Point " << node->getPointNum()
                    << " has topological number "
                    << node->getTopNum() << endl;
            }
        }
    }

    void unweighted(int startPointNum)
    {
        resetShortestPathData();

        GraphNode* startNode = findNode(startPointNum);

        if (startNode == nullptr)
        {
            return;
        }

        queue<GraphNode*> nodeQueue;
        startNode->setDist(0);
        nodeQueue.push(startNode);

        while (!nodeQueue.empty())
        {
            GraphNode* currentNode = nodeQueue.front();
            nodeQueue.pop();

            for (GraphNode* adjacentNode : currentNode->getAdjacentNodes())
            {
                if (adjacentNode->getDist() == numeric_limits<int>::max())
                {
                    adjacentNode->setDist(currentNode->getDist() + 1);
                    adjacentNode->setPath(currentNode);
                    nodeQueue.push(adjacentNode);
                }
            }
        }
    }

    void displayShortestPath(int startPointNum, int endPointNum) const
    {
        GraphNode* startNode = findNode(startPointNum);
        GraphNode* endNode = findNode(endPointNum);

        if (startNode == nullptr || endNode == nullptr)
        {
            cout << "\nInvalid starting or ending point number.\n";
            return;
        }

        if (endNode->getDist() == numeric_limits<int>::max())
        {
            cout << "\nThere is no path from point "
                << startPointNum << " to point "
                << endPointNum << ".\n";
            return;
        }

        vector<int> shortestPath;
        GraphNode* currentNode = endNode;

        while (currentNode != nullptr)
        {
            shortestPath.push_back(currentNode->getPointNum());

            if (currentNode == startNode)
            {
                break;
            }

            currentNode = currentNode->getPath();
        }

        reverse(shortestPath.begin(), shortestPath.end());

        cout << "\nUnweighted shortest path results:\n";
        cout << "Start node: " << startPointNum << endl;
        cout << "End node: " << endPointNum << endl;
        cout << "Distance: " << endNode->getDist() << endl;
        cout << "Path: ";

        for (size_t i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i];

            if (i < shortestPath.size() - 1)
            {
                cout << " -> ";
            }
        }

        cout << endl;
    }
};

int main()
{
    int numberOfNodes;

    cout << "Enter the number of nodes: ";
    cin >> numberOfNodes;

    while (numberOfNodes <= 0)
    {
        cout << "The number of nodes must be greater than zero.\n";
        cout << "Enter the number of nodes: ";
        cin >> numberOfNodes;
    }

    vector<vector<int>> connectivityMatrix(
        numberOfNodes,
        vector<int>(numberOfNodes));

    cout << "\nEnter the " << numberOfNodes << " x "
        << numberOfNodes << " connectivity matrix.\n";
    cout << "Enter 1 when an edge exists and 0 when no edge exists.\n";

    for (int row = 0; row < numberOfNodes; row++)
    {
        cout << "Enter row " << row + 1 << ": ";

        for (int column = 0; column < numberOfNodes; column++)
        {
            cin >> connectivityMatrix[row][column];

            while (connectivityMatrix[row][column] != 0 &&
                connectivityMatrix[row][column] != 1)
            {
                cout << "Please enter only 0 or 1 for row "
                    << row + 1 << ", column "
                    << column + 1 << ": ";
                cin >> connectivityMatrix[row][column];
            }
        }
    }

    Graph graph(connectivityMatrix);
    graph.displayGraph();

    // Retains the Project 11 topological-sort functionality.
    graph.topsort();

    int startPointNum;
    int endPointNum;

    cout << "\nEnter the starting point number: ";
    cin >> startPointNum;

    while (startPointNum < 1 || startPointNum > numberOfNodes)
    {
        cout << "Enter a point number between 1 and "
            << numberOfNodes << ": ";
        cin >> startPointNum;
    }

    cout << "Enter the ending point number: ";
    cin >> endPointNum;

    while (endPointNum < 1 || endPointNum > numberOfNodes)
    {
        cout << "Enter a point number between 1 and "
            << numberOfNodes << ": ";
        cin >> endPointNum;
    }

    graph.unweighted(startPointNum);
    graph.displayShortestPath(startPointNum, endPointNum);

    return 0;
}