#include "aco_ant.hpp"
#include "aco_nodes.hpp"
#include <cmath>
#include <random>

#ifndef aco_as_hpp
#define aco_as_hpp

class aco_as
{
private:
    std::default_random_engine _re;
    std::uniform_int_distribution<int> _ud;
public:
    /**
     * Best path vector, saves history ant best path on the top
     */
    std::vector<std::vector<int>> T;
    /**
     * Best path cost vector, saves history ant best path on the top
     */
    std::vector<float> L;
    /**
     * Best path for current step
     */
    std::vector<int> TminCurrent;
    /**
     * Best path cost for current step
     */
    float LminCurrent;
    
    float feromone;
    std::vector<aco_ant> antPopulation;
    aco_nodes nodes;


    aco_as(
        float feromonePerCycle, 
        int population, 
        float initialAlpha, 
        float initialEvaporation, 
        float feromoneWeight, 
        float costWeight, 
        std::vector<float> & costMatrix
    )
    :
    _ud(0, (int)sqrt(costMatrix.size()) - 1),
    feromone(feromonePerCycle),
    nodes(initialAlpha, initialEvaporation, feromoneWeight,  costWeight, costMatrix)
    {
        for (size_t i = 0; i < population; i++)
        {
            const int initialPosition = _ud(_re);
            antPopulation.emplace_back(feromone);
        }
        
    };
    void reset(float feromonePerCycle)
    {
        feromone = feromonePerCycle;
        for (auto &&ant : antPopulation)
        {
            const int initialPosition = _ud(_re);
            ant.reset(initialPosition, feromone);
        }
    };
    void step()
    {
        if (nodes.costMatrix.size() == 0)
        {
            return;
        }
        
        std::vector<float> pPdf;
        LminCurrent = std::numeric_limits<float>::infinity();
        /**
         * Reset ant population
         */
        reset(feromone);
        /**
         * Generate paths for rest of the nodes
         */
        for (size_t i = 0; i < nodes.stride - 1; i++)
        {
            for (auto &&ant : antPopulation)
            {
                nodes.getPDF(ant.tabu, pPdf);
                ant.nextPosition(pPdf);
            }
        }
        /**
         * Save best path
         */
        for (auto &&ant : antPopulation)
        {
            const float Lant = nodes.getLengthOfPath(ant.tabu);
            if(LminCurrent > Lant)
            {
                LminCurrent = Lant;
                TminCurrent = ant.tabu;
            }
        }
        /**
         * Add up all feromone deltas
         */
        for (auto &&ant : antPopulation)
        {
            nodes.appendFeromoneDeltaForPath(ant.feromone, ant.tabu);
        }
        nodes.addDelta();
        nodes.clearFeromoneDeltaMatrix();
        
    };
    void doCycle()
    {
        step();
        if (((L.size() == 0)?std::numeric_limits<float>::infinity():L.back()) > LminCurrent)
        {
            L.push_back(LminCurrent);
            T.push_back(TminCurrent);
        }
    }
    void doCycles(int cycles)
    {
        for (size_t i = 0; i < cycles; i++)
        {
            doCycle();
        }
    };
    ~aco_as()
    {

    };
};

#endif
