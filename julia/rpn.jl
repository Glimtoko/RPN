const operatorMap = Dict{String, Function}

struct RPN{T<:Number}
    type
    operators::operatorMap

    function RPN{T}() where T<:Number
        type = T
        operators = Dict(
            "+" => (a, b) -> a+b,
            "-" => (a, b) -> a-b,
            "*" => (a, b) -> a*b,
            "x" => (a, b) -> a*b,
            "/" => (a, b) -> a/b
        )
        new(type, operators)
    end

    function RPN{T}(opMap::operatorMap, overwrite::Bool=false) where T<:Number
        type = T
        if overwrite
            operators = operatorMap
        else
            operators = Dict(
                "+" => (a, b) -> a+b,
                "-" => (a, b) -> a-b,
                "*" => (a, b) -> a*b,
                "x" => (a, b) -> a*b,
                "/" => (a, b) -> a/b
            )
            operators = merge(operators, opMap)
        end
        new(type, operators)
    end
end

function parseRPN(rpn::RPN{T}, string::String) where{T <:Number}
    stack = Array{rpn.type}(undef,0)

    for element in split(string, " ")
        if element in keys(rpn.operators)
            f = rpn.operators[element]

            args = Array{rpn.type}(undef,0)
            for i in 1:methods(f).ms[1].nargs - 1
                push!(args, pop!(stack))
            end
            push!(stack, f(args...))
        else
            push!(stack, parse(rpn.type, element))
        end
    end
    pop!(stack)
end

rpnBase = RPN{Int32}()
println(parseRPN(rpnBase, "2 3 4 * +"))

opMap = operatorMap("@" => (a,b) -> -a+b, "#" => (a,b) -> 3a-b)
rpnExt = RPN{Int32}(opMap, false)
println(parseRPN(rpnExt, "2 3 4 @ #"))

rpnComplex = RPN{Complex{Int32}}()
println(parseRPN(rpnComplex, "2 3+3im 4 * +"))
