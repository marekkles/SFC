#include <cmath>
#include <vector>
#include "cost_matrix_interface.hpp"

#ifndef aco_nodes_hpp
#define aco_nodes_hpp

static int aco_find(std::vector<int> arr, int val)
{
    int j = 0;
    for (auto &&i : arr)
    {
        if(val == i)
        {
            return j;
        }
        j++;
    }
    return -1;
}
class aco_nodes : public cost_matrix_interface
{
private:
public:
    std::vector<float> feromoneDeltaMatrix;
    std::vector<float> feromoneMatrix;
    float evaporation;
    float feromoneWeight;
    float costWeight;
    int stride;
    /**
     * Get probability of paths, with given tabel tabu and output vector pProb
     */
    void getPDF(std::vector<int> & tabu, std::vector<float> & pProb)
    {
        int x(tabu.back());
        float sum = 0;
        pProb.clear();
        for (int i = 0; i < stride; i++)
        {
            if(aco_find(tabu, i) == -1)
            {
                sum += pow(getFeromone(x, i), feromoneWeight) * pow(1/getCost(x, i), costWeight);
            }
        }
        for (int i = 0; i < stride; i++)
        {
            if(aco_find(tabu, i) == -1)
            {
                pProb.push_back(((i == 0)?0.0:pProb[i-1]) +  pow(getFeromone(x, i), feromoneWeight) * pow(1/getCost(x, i), costWeight) / sum);
            }
            else
            {
                pProb.push_back((i == 0)?0.0:pProb[i-1]);
            }
        }
    }
    /**
     * Get feromone from x to y
     */
    inline float getFeromone(int x, int y)
    {
        return feromoneMatrix[y*stride + x];
    }
    /**
     * Get feromone from x to y
     */
    inline float getFeromoneDelta(int x, int y)
    {
        return feromoneDeltaMatrix[y*stride + x];
    }
    /**
     * Get cost of path from x to y
     */
    inline float getCost(int x, int y)
    {
        return costMatrix[y*stride + x];
    }
    /**
     * Get length of path for given tabu list
     */
    float getLengthOfPath(std::vector<int>  & tabu)
    {
        float sum = getCost(tabu.front(), tabu.back());
        int previous = tabu.front();
        for (auto i = tabu.begin() + 1; i != tabu.end(); i++)
        {
            int next = *i;
            sum += getCost(previous, next);
            previous = next;
        }
        return sum;
    }
    /**
     * Set value of delta for specific path
     */
    inline void setFeromoneDelta(int x, int y, float value)
    {
        feromoneDeltaMatrix[y*stride + x] = value;
        feromoneDeltaMatrix[x*stride + y] = value;
    }
    /**
     * Append feromone delta according to given path and value of feromone
     */
    void appendFeromoneDeltaForPath(float feromone, std::vector<int> tabu)
    {
        const float length = getLengthOfPath(tabu);
        const float visibility = 1/length;
        const float feromoneDelta = feromone/length;
        setFeromoneDelta(tabu.front(), tabu.back(), getFeromone(tabu.front(), tabu.back()) + feromoneDelta);
        int previous = tabu.front();
        for (auto i = tabu.begin() + 1; i != tabu.end(); i++)
        {
            int next = *i;
            setFeromoneDelta(previous, next, getFeromoneDelta(previous, next) + feromoneDelta);
            previous = next;
        }
    }
    /**
     * Set feromone matrix with value alpha
     */
    void clearFeromoneMatrix(float alpha)
    {
        for (size_t i = 0; i < feromoneMatrix.size() ; i++)
        {
            feromoneMatrix[i] = alpha;
        }
    }
    /**
     * Clear matrix of feromone deltas
     */
    void clearFeromoneDeltaMatrix()
    {
        for (size_t i = 0; i < feromoneDeltaMatrix.size() ; i++)
        {
            feromoneDeltaMatrix[i] = 0;
        }   
    }
    /**
     * Clear matrix of feromone deltas
     */
    void addDelta()
    { 
        for (size_t i = 0; i < feromoneDeltaMatrix.size() && i < feromoneMatrix.size() ; i++)
        {
            feromoneMatrix[i] = feromoneMatrix[i]*(1-evaporation) + feromoneDeltaMatrix[i];
        }
    }
    aco_nodes(float intialAlpha, float intialEvaporation, float weightFeromone,  float weightCost, std::vector<float> inputCostMatrix) :
    feromoneWeight(weightFeromone),
    costWeight(weightCost),
    evaporation(intialEvaporation),
    cost_matrix_interface(inputCostMatrix),
    feromoneDeltaMatrix(inputCostMatrix.size()),
    feromoneMatrix(inputCostMatrix.size()),
    stride((int)sqrt(inputCostMatrix.size()))
    {
        clearFeromoneDeltaMatrix();
        clearFeromoneMatrix(intialAlpha);
    };
    ~aco_nodes()
    {

    };
};
#endif