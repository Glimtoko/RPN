// Import HashMap data type, and the operator (ops) traits
use std::collections::HashMap;
use std::ops;

// Define binary and unary operator traits corresponding to function calls
type BinaryOperator<T> = fn(T, T) -> T;
type UnaryOperator<T> = fn(T) -> T;

// Maps linking Strings to binary/unary operators
type BinaryOpList<T> = HashMap<String, BinaryOperator<T>>;
type UnaryOpList<T> = HashMap<String, UnaryOperator<T>>;

// RPN structure which contains operator lists
struct ReversePolishNotation<T> {
    binary_operators: HashMap<String, BinaryOperator<T>>,
    unary_operators: HashMap<String, UnaryOperator<T>>,
}

/*
    Implimentation block defines the methods of ReversePolishNotation.

    "Constructors" for  ReversePolishNotation:
        Note that Rust does not support function  overloading, and its support 
        for optional arguments is awkward. Thus we'll define two  "constructors"; 
        one will take no arguments, and the other will take three (which will 
        either be None or Some()).
*/
impl<T> ReversePolishNotation<T> 
    // Confirm that T supports basic maths, and conversion from a string
    where T: ops::Add<Output=T> + ops::Sub<Output=T> + ops::Div<Output=T> + ops::Mul<Output=T> + std::str::FromStr
    {
    pub fn base() -> Self {
        // Base constructor using default binary operators
        let mut binary_operators: BinaryOpList<T> = HashMap::new();
        let unary_operators: UnaryOpList<T> = HashMap::new();
        binary_operators.insert("+".to_string(), |a, b| a + b);
        binary_operators.insert("-".to_string(), |a, b| a - b);
        binary_operators.insert("/".to_string(), |a, b| a / b);
        binary_operators.insert("*".to_string(), |a, b| a * b);
        binary_operators.insert("x".to_string(), |a, b| a * b);

        Self {
            binary_operators,
            unary_operators,
        }
    }

    pub fn extended(
        extra_binary_operators_in: Option<BinaryOpList<T>>,
        unary_operators_in: Option<UnaryOpList<T>>,
        replacement_binary_operators_in: Option<BinaryOpList<T>>,

    ) -> Self {
        // Extended constructor

        /*
            N.b. the basic logic used here is that Some(None) returns false,
            whereas Some(Some(X)) returns true
        */
        let mut binary_operators: BinaryOpList<T>;
        if let Some(binary) = replacement_binary_operators_in {
            binary_operators = binary;
        } else {
            binary_operators = HashMap::new();
            binary_operators.insert("+".to_string(), |a, b| a + b);
            binary_operators.insert("-".to_string(), |a, b| a - b);
            binary_operators.insert("/".to_string(), |a, b| a / b);
            binary_operators.insert("*".to_string(), |a, b| a * b);
            binary_operators.insert("x".to_string(), |a, b| a * b);
        }

        let unary_operators: UnaryOpList<T>;
        if let Some(unary) = unary_operators_in {
            unary_operators = unary;
        } else {
            unary_operators = HashMap::new();
        }

        if let Some(extra) = extra_binary_operators_in {
            for (key, value) in extra.iter() {
                // We have to convert key (back) into a string,
                // and dereference value so we can borrow it.
                binary_operators.insert(key.to_string(), *value);
            }
        }

        Self {
            binary_operators,
            unary_operators,
        }
    }

    pub fn parse(&self, string: String) -> T 
    where T: std::str::FromStr, <T as std::str::FromStr>::Err : std::fmt::Debug
    {
        // Stack - Standard Rust vectors support a pop() method
        let mut stack: Vec<T> = Vec::new();

        // Loop over components of string
        for token in string.split_whitespace() {
            // get() method for a HashMap returns an Option<T>, so we can test
            // for Some or None
            let func = self.binary_operators.get(token);
            match func {
                Some(func) => {
                    // pop() method also returns an Option<T>
                    let b = match stack.pop() {
                        Some(v) => v,
                        None => { panic!("Encountered operator, but no value on stack!"); }
                    };

                    let a = match stack.pop() {
                        Some(v) => v,
                        None => { panic!("Encountered operator, but no value on stack!"); }
                    };

                    let value = func(a, b);
                    stack.push(value);
                }
                None => { 
                    let func = self.unary_operators.get(token);
                    match func {
                        Some(func) => {
                            let a = match stack.pop() {
                                Some(v) => v,
                                None => { panic!("Encountered operator, but no value on stack!"); }
                            };
            
                            let value = func(a);
                            stack.push(value);
                        }
                        None => {
                            // unwrap() method is just a quick mechanism for returning data from an
                            // Option<T>
                            let value: T = token.parse().unwrap();
                            stack.push(value);
                        }
                    }
                }
            };
        }

        return match stack.pop() {
            Some(v) => v,
            None => { panic!("No data left on stack at end of evaluation!"); }
        };   
    }
}


fn main() {
    let rpn_base = ReversePolishNotation::<f64>::base();
    let result = rpn_base.parse(String::from("2 3 4 * +"));
    println!("{}", result);

    let mut unary: UnaryOpList<i32> = UnaryOpList::new();
    unary.insert(String::from("@"), |a| 8*a);
    let rpn_ext = ReversePolishNotation::<i32>::extended(None, Some(unary), None);

    /*
        N.b. At this point, unary no longer points to any data, as we passed ownership
        to rpn_ext, rather than letting rpn_ext borrow it.
    */

    let result = rpn_ext.parse(String::from("2 3 4 @ * +"));
    println!("{}", result);
}
