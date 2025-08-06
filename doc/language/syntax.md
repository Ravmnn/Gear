Torque's full syntax in EBNF (Extended Backus-Naur Form) format:


### Basic Syntax

```EBNF

letter = ("a" | "b" | "c" | ... | "z") | ("A" | "B" | "C" | ... | "Z" | "_");
digit = "1" | "2" | "3" | ... | "9" | "0";
digits = digit, { digit };

identifier = letter, { letter | digits };



```


### Expressions

```EBNF

number = digits;
boolean = "true" | "false";

expression = literal | binary;

binary = expression, "+" | "-" | "*" | "/", expression;

literal = number | boolean | identifier;

```