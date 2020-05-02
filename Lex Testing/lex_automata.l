%{

%}

%%
[\n|\r|" "][+|-]?[0-9]+[.[0-9]+]?(E[+|-]?[0-9]+)?[\n|\r|" "] { printf("it's a number\n"); }

'[^('|\n|\r)]*' { printf("it's a string\n"); }

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}