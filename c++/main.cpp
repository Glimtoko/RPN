#include <iostream>
#include <complex>

#include "rpn.h"

// Example user function for a binary operator
double test(double a, double b) { return a + a - b; }

// Example user function for a unary operator
std::complex<float> conjugate(std::complex<float> a) { return std::conj(a); }


int main()
{
    // Define a basic RPN class operating on doubles
    RPN::ReversePolishNotation<double> rpnBasic;
    std::cout << rpnBasic.parse("4 2 5 * + 1 3 2 * + /") << std::endl;

    // Define an RPN class operating on doubles but extended to include a custom operator
    RPN::binaryOpMap<double> operators = { {"@", test} };
    RPN::ReversePolishNotation<double> rpnExtended(operators);
    std::cout << rpnExtended.parse("-4 2 @") << std::endl;

    // Define an RPN class operating on complex floats (<std::complex<float>>)
    RPN::unaryOpMap<std::complex<float>> uOperators = { {"C", conjugate} };
    RPN::ReversePolishNotation<std::complex<float>> rpnComplex(uOperators);
    std::cout << rpnComplex.parse("(2,2) (0,4) *") << std::endl;
    std::cout << rpnComplex.parse("(2,2) C") << std::endl;
    std::cout << rpnComplex.parse("(2,2) (0,4) * C") << std::endl;

    // Test of error case - missing a numerical value before + operator. This will abort the run.
    std::cout << rpnBasic.parse("4 +") << std::endl;
}
