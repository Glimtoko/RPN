import operator
import inspect

class RPN():
    __operators = {}
    __conversion = int

    def __init__(self, conversion=int, operators=None, overwrite=False):           
        self.__operators = {
            "+": operator.add,
            "-": operator.sub,
            "*": operator.mul,
            "x": operator.mul,
            "/": operator.truediv
        }
        
        self.__conversion = conversion
        
        if operators is not None:
            if overwrite:
                self.__operators = operators
            else:
                self.__operators.update(operators)
        
    def parse(self, string):
        stack = []
        for s in string.split():
            if s in self.__operators:
                f = self.__operators[s]
                sig = inspect.signature(f)
                args = []
                for i in range(len(sig.parameters)):
                    args.append(stack.pop())
                val = f(*args)
                stack.append(val)
            else:
                v = self.__conversion(s)
                stack.append(v)
                
        if len(stack) != 1:
            pass
        
        return stack.pop()
