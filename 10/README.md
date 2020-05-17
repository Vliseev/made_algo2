Описание грамматики
<pre>
assign_list: assign | assign assign_list
assign: var ASSIGN expr
expr: term ((PLUS | MINUS) term)*
term: factor ((MUL | DIV) factor)*
factor: PLUS factor
        | MINUS factor
        | NUM
        | LPARENT expr RPARENT
        | var
var: id
</pre>
Пример использования:
```
python3 simple_interpreter.py
enter file name>example_inp.txt
```
Тесты:
```
python3 -m unittest test_interpreter.py
python3 -m unittest test_parser.py
```