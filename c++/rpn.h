/*
Header-only library (since it's all templated) defining a ReversePolishNotation class
which can parse an arbitrary string in RPN form. Will work for any data type which
supports deserialisation (>>) from a std::stringstream.

ReversePolishNotation is placed within the RPN namespace to avoid polluting global scope.

N.b. tested with:
    g++ 9.3
    clang++ 10.0.1
    MS Visual C++ 2019
*/
#ifndef RPNHEADER
#define RPNHEADER

#include <iostream>
#include <sstream>
#include <iterator>
#include <map>
#include <stack>
#include <math.h>

namespace RPN {
    // Alias declaration of a std::map between a std::string and a general function pointer
    template <typename T>
    using binaryOpMap = std::map <std::string, T(*)(T, T)>;

    template <typename T>
    using unaryOpMap = std::map <std::string, T(*)(T)>;

    template <typename T>
    class ReversePolishNotation {
    private:
        binaryOpMap<T> binaryOperators;
        unaryOpMap<T> unaryOperators;
    public:
        // Default constructor sets binaryOperators to the basic maths functions
        ReversePolishNotation<T>()
            : binaryOperators{ { 
                {"+", [](T a, T b) {return a + b; }}, 
                {"-", [](T a, T b) {return a - b; }}, 
                {"*", [](T a, T b) {return a * b; }}, 
                {"x", [](T a, T b) {return a * b; }}, 
                {"/", [](T a, T b) {return a / b; }}, 
                {"^", [](T a, T b) {return pow(a, b); }}
            } }
        {}

        // Extended constructor allows user to supplement, or replace, the default operator list
        ReversePolishNotation<T>(binaryOpMap<T> extrabinaryOperators, bool override=false)
            : ReversePolishNotation()
        {
            if (override) {
                binaryOperators = extrabinaryOperators;
            } else {
                binaryOperators.insert(extrabinaryOperators.begin(), extrabinaryOperators.end());
            }
        }

        // Constructor allowing definition of unary operators
        ReversePolishNotation<T>(unaryOpMap<T> unaryOperatorsIn)
            : ReversePolishNotation()
        {
            unaryOperators = unaryOperatorsIn;
        }

        // Extended constructor allows user to supplement, or replace, the default operator list, and definition of unary operators
        ReversePolishNotation<T>(binaryOpMap<T> extrabinaryOperators, unaryOpMap<T> unaryOperators, bool override=false)
            : ReversePolishNotation()
        {
            unaryOperators = unaryOperators;
            if (override) {
                binaryOperators = extrabinaryOperators;
            } else {
                binaryOperators.insert(extrabinaryOperators.begin(), extrabinaryOperators.end());
            }
        }


        T parse(std::string input);
    };

}

template <typename T>
T RPN::ReversePolishNotation<T>::parse(std::string input)
{
    /*
    Interpret a string in RPN form. Use a stack object to store intermediate values, so
    they can be popped whenever an operator is encountered.
    */
    std::stack<T> theStack;

    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        if (binaryOperators.find(token) != binaryOperators.end()) {
            // Encountered an operator. Attempt to pop the top two values from the stack
            if (theStack.empty()) throw std::runtime_error("Error: Invalid RPN string: '" + input + "'");
            T b = theStack.top();
            theStack.pop();

            if (theStack.empty()) throw std::runtime_error("Error: Invalid RPN string: '" + input + "'");
            T a = theStack.top();
            theStack.pop();

            // Apply operator, and push result back to stack
            theStack.push(binaryOperators[token](a, b));
        } else if (unaryOperators.find(token) != unaryOperators.end()) {
            if (theStack.empty()) throw std::runtime_error("Error: Invalid RPN string: '" + input + "'");
            T a = theStack.top();
            theStack.pop();

            // Apply operator, and push result back to stack
            theStack.push(unaryOperators[token](a));
        } else {
            // Non operator. Attempt to convert to type T
            std::stringstream tss(token);
            T val = 0;
            tss >> val;

            // Detect failed conversion
            if (tss.fail()) throw std::runtime_error("Error: Invalid token '" + token + "'");

            // Push value to stack
            theStack.push(val);
        }
    }
    // A valid RPN string should result in only one value, from the final operator, remaining on the stack
    if (theStack.size() != 1) throw std::runtime_error("Error: RPN string did not evaluate to a single number");

    // Return the value
    return theStack.top();
}
#endif
