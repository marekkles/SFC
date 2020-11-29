#include <vector>
#include <array>
#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <algorithm> 

#ifndef tree_nodes_hpp
#define tree_nodes_hpp

class tree_node
{
private:
    /* data */
public:
    float x;
    float y;
    tree_node(float X, float Y) :
    x(X),
    y(Y)
    {}
    ~tree_node() {}
};

class tree_nodes
{
public:
    /**
     * Vector containigng all edges of paths, direction invariant
     */
    std::vector<std::array<int, 2>> edges;
    /**
     * Vector containing all nodes of the tree (graph)
     */
    std::vector<tree_node> nodes;
    /**
     * A vector used to map marked nodes indexes to indexes in nodes vector
     */
    std::vector<int> markedNodes;
    /**
     * Cost of paths between all marked nodes
     */
    std::vector<float> markedCostMatrix;
    /**
     * Vector containig optimal paths between marked nodes
     */
    std::vector<std::vector<int>> markedPaths;
    /**
     * Limits:
     * Xmin, Ymin, Xmax, Ymax
     */
    std::array<float, 4> limits;
    /**
     * Get path between two marked nodes
     */
    void getMarkedPath(int i1, int i2, std::vector<int> &resultPath)
    {
        int id1 = markedNodes[i1];
        int id2 = markedNodes[i2];
        for (auto &&path : markedPaths)
        {
            int co1 = path.front();
            int co2 = path.back();
            if ((co1 == id1 && co2 == id2) || (co1 == id2 && co2 == id1))
            {
                resultPath = path;
            }
        }
    }
    /**
     * Method return a value in markedCostMatrix between two marked nodes with indexes of i1 and i2
     */
    inline float getMarkedCostMatrix(int i1, int i2)
    {
        return markedCostMatrix[markedNodes.size()*i2 + i1];
    }
    /**
     * Method sets a value of path between two marked nodes
     */
    inline void setMarkedCostMatrix(int i1, int i2, float value)
    {
        markedCostMatrix[markedNodes.size()*i2 + i1] = value;
        markedCostMatrix[markedNodes.size()*i1 + i2] = value;
    }
    /**
     * Calculate markedCostMatrix based on A* algorith
     */
    void calculateMarkedCostMatrix()
    {
        markedCostMatrix.clear();
        markedCostMatrix.resize(markedNodes.size()*markedNodes.size());
        for (size_t i = 0; i < markedNodes.size(); i++)
        {
            for (size_t j = 0; j <= i; j++)
            {
                markedPaths.emplace_back();
                float cost = A_star_path(markedNodes[i], markedNodes[j], markedPaths.back());
                setMarkedCostMatrix(i, j, cost);
            }
        }
    }
    /**
     * Get neighbours of certain node
     */
    void getNeighbours(int i, std::vector<int> &neighbours)
    {
        for (auto &&edge : edges)
        {
            if (edge[0] == i)
            {
                neighbours.push_back(edge[1]);
            }
            else if (edge[1] == i)
            {
                neighbours.push_back(edge[0]);
            }
        }
    }
    /**
     * Use A* algorith to get optimal path and return cost
     * Heuristic is based on euclidean distance
     */
    float A_star_path(int i1, int i2, std::vector<int> &path)
    {
        path.clear();
        std::map<int, int> from;
        std::map<int, float> g_score;
        std::map<int, float> f_score;
        std::set<int> open;

        g_score[i1] = 0.0;
        f_score[i1] = getEdgeLength(i1, i2);

        open.insert(i1);

        while (open.size() != 0)
        {
            float currentScore = std::numeric_limits<float>::infinity();
            int current = 0;
            //find best node
            for (auto &&node : open)
            {
                if(f_score[node] < currentScore)
                {
                    currentScore = f_score[node];
                    current = node;
                }
            }
            //reconstruct path
            if (current == i2)
            {
                int last = i2;
                while (last != i1)
                {
                    path.push_back(last);
                    last = from[last];
                }
                path.push_back(last);
                std::reverse(path.begin(), path.end());
                return currentScore;
            }

            open.erase(current);

            std::vector<int> neighbours;
            getNeighbours(current, neighbours);
            
            for (auto &&neighbour : neighbours)
            {
                float temp_g_Score = g_score[current] + getEdgeLength(current, neighbour);
                float recorded_g_score = (g_score.find(neighbour) != g_score.end())?g_score[neighbour]:std::numeric_limits<float>::infinity();
                if(recorded_g_score > temp_g_Score)
                {
                    g_score[neighbour] = temp_g_Score;
                    f_score[neighbour] = temp_g_Score + getEdgeLength(neighbour, i2);
                    from[neighbour] = current;
                    if (open.find(neighbour) == open.end())
                    {
                        open.insert(neighbour);
                    }
                }
            }

        }
    }
    /**
     * Get distance between two points
     */
    inline float getLength(float p1x, float p2x, float p1y, float p2y)
    {
        return std::sqrt(
            (p1x - p2x)*(p1x - p2x) +
            (p1y - p2y)*(p1y - p2y)
        );
    }
    /**
     * Get length of specified edge
     */
    inline float getEdgeLength(int i1, int i2)
    {
        return getLength(nodes[i1].x, nodes[i2].x, nodes[i1].y, nodes[i2].y);
    }
    /**
     * Check if point is within bounds of two nodes
     */
    inline bool isWithinBounds(int i1, int i2, float x, float y)
    {
        float deltaX, deltaY, deltaIX, deltaIY;
        deltaX = std::abs(x - nodes[i1].x) + std::abs(x - nodes[i2].x);
        deltaY = std::abs(y - nodes[i1].x) + std::abs(y - nodes[i2].x);
        deltaIX = std::abs(nodes[i1].x - nodes[i2].x);
        deltaIY = std::abs(nodes[i1].y - nodes[i2].y);
        return ((deltaX <= deltaIX + 20) && (deltaY <= deltaIY + 20));
    }
    /**
     * Get distance of point from edge defined by two points
     */
    inline float getFieldEffectOfEdge(int i1, int i2, float x, float y)
    {
        tree_node n1(nodes[i1]);
        tree_node n2(nodes[i2]);
        return 
        (
            std::sqrt(
                (x - n1.x)*(x - n1.x) +
                (y - n1.y)*(y - n1.y)
            )
            +
            std::sqrt(
                (x - n2.x)*(x - n2.x) +
                (y - n2.y)*(y - n2.y)
            )
        )
        /
        std::sqrt(
            (n2.x - n1.x)*(n2.x - n1.x) +
            (n2.y - n1.y)*(n2.y - n1.y)
        );
    }
    /**
     * Find closest edge to a point, returns index of edge in edges vector
     */
    int findClosestEdge(float x, float y)
    {
        int closestEdge = -1;
        float smallestFieldStrength = std::numeric_limits<float>::infinity();
        int i = 0;
        for (size_t i = 0; i < edges.size(); i++)
        {
            const auto edge(edges[i]);
            const float currentFieldStrength = getFieldEffectOfEdge(edge[0], edge[1], x, y);
            if (currentFieldStrength < smallestFieldStrength)
            {
                closestEdge = i;
                smallestFieldStrength = currentFieldStrength;
            }
        }
        return closestEdge;
    }
    /**
     * Project point onto an edge
     */
    void projectOntoEdge(int i1, int i2, float x, float y, float & xp, float & yp)
    {
        const float dI1 = std::sqrt((x - nodes[i1].x)*(x - nodes[i1].x) + (y - nodes[i1].y)*(y - nodes[i1].y));
        const float dI2 = std::sqrt((x - nodes[i2].x)*(x - nodes[i2].x) + (y - nodes[i2].y)*(y - nodes[i2].y));
        const float len = getEdgeLength(i1, i2);
        const float t = dI1/(dI1 + dI2);
        xp = t*(nodes[i2].x - nodes[i1].x) + nodes[i1].x;
        yp = t*(nodes[i2].y - nodes[i1].y) + nodes[i1].y;
    }
    /**
     * Add new marked node to position, it is snapped to a closest edge,
     * if no edge is found false is returned, true otherwise
     */
    bool addMarkedNode(float x, float y)
    {
        const int closestEdge = findClosestEdge(x, y);
        if(!closestEdge != -1)
        {
            nodes.emplace_back(x, y);
            projectOntoEdge(edges[closestEdge][0], edges[closestEdge][1], x, y, nodes.back().x, nodes.back().y);
            
            int i2 = edges[closestEdge][1];
            edges[closestEdge][1] = nodes.size() - 1;

            std::array<int, 2> newEdge{{static_cast<int>(nodes.size()) - 1, i2}};
            edges.push_back(newEdge);
            markedNodes.emplace_back(nodes.size() - 1);
        }
        else
        {
            return false;
        }
    }
    /**
     * Reset all variables
     */
    void reset()
    {
        edges.clear();
        nodes.clear();
        markedNodes.clear();
        markedCostMatrix.clear();
        markedPaths.clear();
    }
    /**
     * Parse basic tree from input stream
     */
    void parse(std::istream & stream)
    {
        //Create state controll
        typedef enum {
            POINTS,
            EDGES,
            MARKED
        } state_t;
        state_t state = POINTS;
        //Reset limits
        limits[0] = std::numeric_limits<float>::infinity();
        limits[1] = std::numeric_limits<float>::infinity();
        limits[2] = -std::numeric_limits<float>::infinity();
        limits[3] = -std::numeric_limits<float>::infinity();

        while (!stream.eof())
        {
            std::string word;
            stream >> word;
            if (state == POINTS)
            {
                if (word == "E")
                {
                    state = EDGES;
                }
                else
                {
                    float x, y;
                    bool marked;
                    std::stringstream word_stream;
                    word_stream.str(word);
                    word_stream >> x;
                    stream >> y;
                    nodes.emplace_back(x, y);
                    //Save limits
                    limits[0] = (x < limits[0])?x:limits[0];
                    limits[1] = (y < limits[1])?y:limits[1];
                    limits[2] = (x > limits[2])?x:limits[2];
                    limits[3] = (y > limits[3])?y:limits[3];
                }
            }
            else if (state == EDGES)
            {
                if (word == "#")
                {
                    state = MARKED;
                }
                else
                {
                    int i1, i2;
                    std::stringstream word_stream;
                    word_stream.str(word);
                    word_stream >> i1;
                    stream >> i2;
                    std::array<int, 2> newEdge{{i1, i2}};
                    edges.push_back(newEdge);
                }
            }
            else
            {
                int m;
                stream >> m;
                markedNodes.push_back(m);
            }
        }
    }
    void save(std::ostream & stream)
    {
        int j = 0;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            stream << nodes[i].x << " " << nodes[i].y << std::endl;
        }
        stream << "E" << std::endl;
        for (auto &&edge : edges)
        {
            stream << edge[0] << " " << edge[1] << std::endl;
        }
        if (markedNodes.size() != 0)
        {
            stream << "#" << std::endl;
            for (auto &&markedNode : markedNodes)
            {
                stream << markedNode << std::endl;
            }
        }
        
    }
    tree_nodes(/* args */) {

    }
    ~tree_nodes() {}
};

#endif