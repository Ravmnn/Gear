Torque's full syntax in EBNF (Extended Backus-Naur Form) format:


### Basic Syntax

```EBNF

letter = ("a" | "b" | "c" | ... | "z") | ("A" | "B" | "C" | ... | "Z" | "_");
digit = "1" | "2" | "3" | ... | "9" | "0";
digits = digit, { digit };

identifier = letter, { letter | digits };
type_identifier = identifier;

```


### Expressions

```EBNF

number = digits;
boolean = "true" | "false";


expression = binary_expression | call_expression | cast_expression | identifier_expression |
                grouping_expression | literal_expression;

binary_expression = expression, "+" | "-" | "*" | "/", expression;
call_expression = identifier_expression, "(", [ expression, { ",", expression } ], ")";
cast_expression = expression, "as", type_identifier;

identifier_expression = identifier;
grouping_expression = "(", expression, ")";
literal_expression = number | boolean;

```



### Statements

```EBNF

statement = block_statement | expression_statement | variable_declaration;
block_statement = "start", { statement }, "end";

expression_statement = expression, ";";

variable_declaration = type_identifier, identifier, "=", expression, ";";

parameter_declaration = type_identifier, identifier;
function_declaration = type_identifier, identifier,
                            "(", [ parameter_declaration, { ",", parameter_declaration } ], ")",
                            block_statement;

return_statement = "return", expression, ";";


```