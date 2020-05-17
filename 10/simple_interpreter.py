from interpreter import Interpreter
from parser import tokenizer, Parser


def interpret_text(text: str):
    lexer = tokenizer(text)
    parser = Parser(lexer)

    assign_list = parser.assign_list()
    interpreter = Interpreter()

    interpreter.interp(assign_list)
    return interpreter.variable_list


if __name__ == '__main__':
    fname = input("enter file name>")
    with open(fname) as f:
        inp = f.read()

    variables = interpret_text(inp)
    for k, v in variables.items():
        print(f"{k}={v}")
