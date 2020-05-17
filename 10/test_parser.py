from unittest import TestCase
import unittest
from parser import Parser, tokenizer


class TestParser(TestCase):
    def test_parse_test1(self):
        text = "c = 2 * 5"
        lexer = tokenizer(text)
        parser = Parser(lexer)
        assign_res = parser.assign_list()
        self.assertEqual(len(assign_res), 1)

    def test_parse_test2(self):
        text = """
        c=2*5*4
        a=(c*7)+7
        """
        lexer = tokenizer(text)
        parser = Parser(lexer)
        assign_res = parser.assign_list()
        self.assertEqual(len(assign_res), 2)

    def test_invalid_syntax1(self):
        text = """
               1c=2*5*4
               a=c*7+7
               """
        lexer = tokenizer(text)
        parser = Parser(lexer)

        exception_t = None
        try:
            assign_res = parser.assign_list()
        except Exception as e:
            exception_t = e

        self.assertEqual(exception_t.args[0], "Invalid syntax")

    def test_invalid_syntax2(self):
        text = """
               a=(2*7+7
               """
        lexer = tokenizer(text)
        parser = Parser(lexer)

        exception = None
        try:
            assign_res = parser.assign_list()
        except Exception as e:
            exception = e

        self.assertEqual(exception.args[0], "Invalid syntax")

if __name__ == '__main__':
    unittest.main()