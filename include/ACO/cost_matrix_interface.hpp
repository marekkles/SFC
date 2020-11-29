#include <istream>
#include <ostream>
#include <sstream>

#ifndef cost_matrix_interface_hpp
#define cost_matrix_interface_hpp

class parse_error
{
private:
    /* data */
public:
    parse_error(/* args */){};
    ~parse_error(){};
};


class cost_matrix_interface
{
private:
    /* data */
public:
    std::vector<float> costMatrix;
    
    void parse(std::istream & stream)
    {
        int col = 0;
        int row = 0;
        costMatrix.clear();
        while (!stream.eof())
        {
            std::stringstream  ss;
            int currentCol = 0;
            std::string line;
            std::getline(stream, line);
            ss.str(line);
            while (!ss.eof() && line.size() != 0)
            {
                float n;
                ss >> n;
                costMatrix.push_back(n);
                currentCol++;
            }
            if (row == 0)
            {
                col = currentCol;
            }
            else if (col != currentCol)
            {
                throw new parse_error();
            }
            row++;
        }
        if (row != col)
        {
            throw new parse_error();
        }
    }
    void save(std::ostream & stream)
    {
        int stride = (int)sqrt(costMatrix.size());
        for (size_t i = 0; i < stride; i++)
        {
            stream << costMatrix[i*stride + 0];
            for (size_t j = 1; j < stride; j++)
            {
                stream << " " << costMatrix[i*stride + j];
            }
            if (i != stride - 1)
            {
                stream << std::endl;
            }
        }
    };
    cost_matrix_interface()
    {
    };
    cost_matrix_interface(std::vector<float> & _costMatrix) :
    costMatrix(_costMatrix)
    {
    };
    cost_matrix_interface(int size):
    costMatrix(size*size)
    {
    };
    virtual ~cost_matrix_interface()
    {
    };
};

#endif
