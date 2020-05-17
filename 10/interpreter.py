from parser import Assign, Var, UnaryOp, PLUS_T, MINUS_T, BinOp, MULT_T, DIVIDE_T, Num


class Interpreter:
    def __init__(self):
        self.func_map = {"Assign": self.visit_assign,
                         "Var": self.visit_var,
                         "UnaryOp": self.visit_unary_op,
                         "BinOp": self.visit_bin_op,
                         "Num": self.visit_num}
        self.variable_list = {}

    def error(self):
        raise Exception('Invalid syntax')

    def visit(self, node):
        node_type = type(node).__name__
        func = self.func_map.get(node_type)
        if func is None:
            self.error()
        return func(node)

    def visit_assign(self, node: Assign):
        var_name = node.left.value
        self.variable_list[var_name] = self.visit(node.right)

    def visit_var(self, node: Var):
        var_name = node.value
        value = self.variable_list.get(var_name)
        if value:
            return value
        else:
            self.error()

    def visit_unary_op(self, node: UnaryOp):
        op = node.op
        if op.type == PLUS_T:
            return self.visit(node.expr)
        elif op.type == MINUS_T:
            return -self.visit(node.expr)

    def visit_bin_op(self, node: BinOp):
        if node.op.type == PLUS_T:
            return self.visit(node.left) + self.visit(node.right)
        elif node.op.type == MINUS_T:
            return self.visit(node.left) - self.visit(node.right)
        elif node.op.type == MULT_T:
            return self.visit(node.left) * self.visit(node.right)
        elif node.op.type == DIVIDE_T:
            return self.visit(node.left) / self.visit(node.right)

    @staticmethod
    def visit_num(node: Num):
        return node.value

    def interp(self, assign_list):
        self.variable_list = {}
        for assign in assign_list:
            self.visit(assign)
