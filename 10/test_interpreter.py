from unittest import TestCase
import unittest

from interpreter import Interpreter
from parser import tokenizer, Parser


def interpr_text(text: str):
    lexer = tokenizer(text)
    parser = Parser(lexer)

    assign_list = parser.assign_list()
    interpreter = Interpreter()

    interpreter.interp(assign_list)
    return interpreter.variable_list


class TestInterpreter(TestCase):
    def test_interp(self):
        text = "c = 2 * 5"
        correct_ans = [('c', 10)]
        ans = interpr_text(text)

        self.assertEqual(len(ans), len(correct_ans))
        for var, val in correct_ans:
            interp_val = ans.get(var)
            self.assertEqual(val, interp_val)

    def test_interp2(self):
        text = """
        c=(-4+8-4)/4+(7+3)*2
        a=(+c+7)*2
        """
        correct_ans = [('c', 20), ('a', 54)]
        ans = interpr_text(text)

        self.assertEqual(len(ans), len(correct_ans))
        for var, val in correct_ans:
            interp_val = ans.get(var)
            self.assertEqual(val, interp_val)

    def test_interp3(self):
        text = """
        c=(-4+8-4)/4+(7+3)*2
        a=(e+7)*2
        """
        try:
            ans = interpr_text(text)
        except Exception as e:
            exception = e

        self.assertEqual(exception.args[0], "Invalid syntax")

    def test_interp4(self):
        text = """
        c=4$7
        """
        try:
            ans = interpr_text(text)
        except Exception as e:
            exception = e

        self.assertEqual(exception.args[0], "Invalid syntax")


if __name__ == '__main__':
    unittest.main()