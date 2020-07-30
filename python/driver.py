import numpy
import rpn      
        
rpnBasic = rpn.RPN(float)
print(rpnBasic.parse("2 3 *"))

rpnComplex = rpn.RPN(complex, operators={"C": lambda a: numpy.conj(a)})
print(rpnComplex.parse("2+1j 7j * C"))