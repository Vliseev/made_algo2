import re

NUM_REG = r'(?P<NUM>\d+)'
PLUS_REG = r'(?P<PLUS>\+)'
MINUS_REG = r'(?P<MINUS>-)'
MULT_REG = r'(?P<MULT>\*)'
DIVIDE_REG = r'(?P<DIVIDE>/)'
LPAREN_REG = r'(?P<LPAREN>\()'
RPAREN_REG = r'(?P<RPAREN>\))'
VAR_REG = r'(?P<VAR>([\w\d^]\w*))'
WS_REG = r'(?P<WS>\s+)'
ASSIGN_REG = r'(?P<ASSIGN>=)'

NUM_T = "NUM"
PLUS_T = "PLUS"
MINUS_T = "MINUS"
MULT_T = "MULT"
DIVIDE_T = "DIVIDE"
LPAREN_T = "LPAREN"
RPAREN_T = "RPAREN"
VAR_T = "VAR"
WS_T = "WS"
ASSIGN_T = "ASSIGN"
EOF_T = "EOF"

BASE_PATTERN = re.compile('|'.join([NUM_REG,
                                    PLUS_REG,
                                    MINUS_REG,
                                    MULT_REG,
                                    DIVIDE_REG,
                                    LPAREN_REG,
                                    RPAREN_REG,
                                    VAR_REG,
                                    WS_REG,
                                    ASSIGN_REG]))


class Token(object):
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({type}, {value})'.format(
            type=self.type,
            value=repr(self.value)
        )

    def __repr__(self):
        return self.__str__()


def tokenizer(text):
    scanner = re.finditer(BASE_PATTERN, text)
    for m in scanner:
        tok = Token(m.lastgroup, m.group())
        if tok.type != 'WS':
            yield tok
    yield Token(EOF_T, None)


class AST(object):
    pass


class BinOp(AST):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right


class Assign(AST):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right


class Num(AST):
    def __init__(self, token):
        self.token = token
        self.value = int(token.value)


class Var(AST):
    def __init__(self, token):
        self.token = token
        self.value = token.value


class UnaryOp(AST):
    def __init__(self, op, expr):
        self.token = self.op = op
        self.expr = expr


class Parser:
    def __init__(self, lexer):
        self.lexer = lexer
        self.cur_tok = next(lexer)

    def error(self):
        raise Exception('Invalid syntax')

    def eat(self, token_type):
        if self.cur_tok.type == token_type:
            self.cur_tok = next(self.lexer)
        else:
            self.error()

    def assign(self):
        left = self.variable()
        assign_tok = self.cur_tok
        self.eat(ASSIGN_T)
        right = self.expression()
        return Assign(left, assign_tok, right)

    def variable(self):
        node = Var(self.cur_tok)
        self.eat(VAR_T)
        return node

    def expression(self):
        node = self.term()
        while self.cur_tok.type in (PLUS_T, MINUS_T):
            token = self.cur_tok
            if token.type == PLUS_T:
                self.eat(PLUS_T)
            elif token.type == MINUS_T:
                self.eat(MINUS_T)

            node = BinOp(left=node, op=token, right=self.term())

        return node

    def factor(self):
        token = self.cur_tok
        if token.type == PLUS_T:
            self.eat(PLUS_T)
            node = UnaryOp(token, self.factor())
            return node
        elif token.type == MINUS_T:
            self.eat(MINUS_T)
            node = UnaryOp(token, self.factor())
            return node
        elif token.type == NUM_T:
            self.eat(NUM_T)
            return Num(token)
        elif token.type == LPAREN_T:
            self.eat(LPAREN_T)
            node = self.expression()
            self.eat(RPAREN_T)
            return node
        else:
            node = self.variable()
            return node

    def term(self):
        node = self.factor()
        while self.cur_tok.type in (MULT_T, DIVIDE_T):
            token = self.cur_tok
            if token.type == MULT_T:
                self.eat(MULT_T)
            elif token.type == DIVIDE_T:
                self.eat(DIVIDE_T)
            node = BinOp(left=node, op=token, right=self.factor())
        return node

    def assign_list(self):
        assign = self.assign()
        assign_list = [assign]
        while not self.cur_tok.type == EOF_T:
            assign_list.append(self.assign())
        return assign_list