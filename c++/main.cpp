#include <iostream>
#include <complex>

#include "rpn.h"

int main()
{
    // Define a basic RPN class operating on doubles
    RPN::ReversePolishNotation<double> rpnBasic;
    std::cout << rpnBasic.parse("4 2 5 * + 1 3 2 * + /") << std::endl;

    // Define an RPN class operating on doubles but extended to include a custom operator
    RPN::binaryOpMap<double> operators = { 
        {"@", [](double a, double b){ return a+a-b; }} 
    };
    RPN::ReversePolishNotation<double> rpnExtended(operators);
    std::cout << rpnExtended.parse("-4 2 @") << std::endl;

    // Define an RPN class operating on complex floats (<std::complex<float>>)
    RPN::unaryOpMap<std::complex<float>> uOperators = { 
        {"C", [](std::complex<float> a) { return std::conj(a); }} 
    };
    RPN::ReversePolishNotation<std::complex<float>> rpnComplex(uOperators);
    std::cout << rpnComplex.parse("(2,2) (0,4) *") << std::endl;
    std::cout << rpnComplex.parse("(2,2) C") << std::endl;
    std::cout << rpnComplex.parse("(2,2) (0,4) * C") << std::endl;

    // Test of error case - missing a numerical value before + operator. This will abort the run.
    std::cout << rpnBasic.parse("4 +") << std::endl;
}
