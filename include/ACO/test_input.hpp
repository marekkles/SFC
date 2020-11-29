#include <vector>
#include <array>
#include <random>
#include "cost_matrix_interface.hpp"

#ifndef test_input_hpp
#define test_input_hpp

class test_input : public cost_matrix_interface
{
private:
    std::default_random_engine _re;
    std::uniform_real_distribution<float> _ud;
public:
    std::vector<std::array<float, 2>> points;
    
    test_input(int size):
    _re(),
    _ud(-100.0, 100.0),
    points(size)
    {
        for (size_t i = 0; i < size; i++)
        {
            points[i][0] = _ud(_re);
            points[i][1] = _ud(_re);
        }
        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                const float val = sqrt(pow(points[i][0] - points[j][0],2) + pow(points[i][1] - points[j][1], 2));
                costMatrix.push_back(val);
            }
        }
    };
    ~test_input()
    {

    };
};

#endif
