#include <vector>
#include <ctime>
#include <random>
#include <cstdlib>

#ifndef aco_ant_hpp
#define aco_ant_hpp

class aco_ant
{
private:
    std::default_random_engine _re;
    std::uniform_real_distribution<float> _ud;
public:
    std::vector<int> tabu;
    std::vector<float> tabuDelta;
    float tabuLength;
    float feromone;

    aco_ant(float feromonePerCycle):
    _ud(0.0, 1.0),
    feromone(feromonePerCycle)
    {
        
    };
    void reset(int initialPosition, float feromonePerCycle)
    {
        feromone = feromonePerCycle;
        tabu.clear();
        tabu.push_back(initialPosition);
    }
    /**
     * Get next position by randomly selecting with probability density of pPdf,
     * newly selected is first index with probability density of less than generated 
     * probability 
     */
    void nextPosition(std::vector<float> & pPdf)
    {
        float prob = _ud(_re) * pPdf.back();
        int candidateIndex = 0;
        for (; candidateIndex < pPdf.size(); candidateIndex++)
        {
            if (prob < pPdf[candidateIndex])
            {
                break;
            }
        }
        // if for some reason inverse lookup does not work, pick first not yet selected
        if (candidateIndex >= pPdf.size())
        {
            for (size_t i = 0; i < pPdf.size(); i++)
            {
                bool isFound = false;
                for (auto &&j : tabu)
                {
                    if(i == j)
                    {
                        isFound = true;
                        break;
                    }
                }
                if (isFound == false)
                {
                    candidateIndex = i;
                    break;
                }
            }
        }
        
        tabu.push_back(candidateIndex);
    };
    ~aco_ant()
    {

    };
};

#endif