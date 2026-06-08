# parser 

## Kern parser is using recursive descent algoritm for parsing and make * AST *


in the parser we take the token stream `lexer->head` and give it to parser `parser->current` and do this steps :

imagen we have this source code 
```c
int main(){
    int num = 7;
    return 42;
}
```
and ofcourse this token streams 

TKN_INT -> TKN_IDENTIFIER -> TKN_L_PRAN -> . . . . TKN_INT_LITTERAL -> TKN_SEMI_COL -> TKN_R_CBRAKC


> in this step we start from the first token `TKN_INT` what's possible tokens after int ?? 
> int is data type in C and after data types we have the name of variable itself OR maybe it's in declaring function and it might be the return type of that func

yea and for all the data types we have this pattern so we call `parse_declaration()` and peek two token forether if we see `TKN_L_PRAN` it's func declaration
and if it's `TKN_EQ` or `TKN_SEMCOL` it's variable declaration
