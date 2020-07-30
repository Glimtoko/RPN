/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.glimtoko.rpn;

import java.util.*;

/**
 *
 * @author nick
 */
public class RPN {
    /* Function pointers in Java use a system not dissimilar to Fortran,
    using an interface.
    
    Fun fact: Java is often accused of using the worst concepts from other
    programming languages, so probably not a surprise Fortran raided it...
    */
    interface BinaryOperation {
        double operation(double a, double b);
    }
    private double getBinaryResult(double a, double b, String s) {
        BinaryOperation op = this.binaryOperators.get(s);
        return op.operation(a, b);
    }
    
    interface UnaryOperation {
        double operation(double a);
    }   
    private double getUnnaryResult(double a, String s) {
        UnaryOperation op = this.unaryOperators.get(s);
        return op.operation(a);
    }

    
    HashMap<String, BinaryOperation> binaryOperators;
    HashMap<String, UnaryOperation> unaryOperators;

    public RPN() {
        this.binaryOperators = new HashMap<>();
        
        BinaryOperation subtract = (double a, double b) -> a-b;
        BinaryOperation add = (double a, double b) -> a+b;
        BinaryOperation mult = (double a, double b) -> a*b;
        BinaryOperation div = (double a, double b) -> a/b;
        
        this.binaryOperators.put("-", subtract);
        this.binaryOperators.put("+", add);
        this.binaryOperators.put("*", mult);
        this.binaryOperators.put("x", mult);
        this.binaryOperators.put("/", div);
        
    }
    
    public RPN(HashMap<String, BinaryOperation> extraOps, boolean overwrite) {
        this();
        if (overwrite) {
            this.binaryOperators = extraOps;
        } else {
            this.binaryOperators.putAll(extraOps);
        }
    }   
    
   
    double parseRPN(String input) {
        Stack<Double> theStack = new Stack<Double>();
        
        for (String s:input.split(" ")) {
            if (binaryOperators.containsKey(s)) {
                double b = theStack.pop();
                double a = theStack.pop();
                theStack.push(this.getBinaryResult(a, b, s));
            } else {
                double v = Double.parseDouble(s);
                theStack.push(v);
            }
        }
        
        return theStack.pop();
    }
    
    public static void main(String[] args) {
        RPN baseRPN = new RPN();
        System.out.println(baseRPN.parseRPN("2 3 4 * +"));

        HashMap<String, BinaryOperation> binaryOperators = new HashMap<>(); 
        BinaryOperation crazy = (double a, double b) -> a+a-b;
        binaryOperators.put("@", crazy);
        RPN extRPN = new RPN(binaryOperators, false);
        System.out.println(extRPN.parseRPN("2 3 4 * @"));
    }
             
}
