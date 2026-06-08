# Lexer module map
---
## functions

| function name |         args         | return |                         description                          |                    borrowed/owned                     |
| :-----------: | :------------------: | :----: | :----------------------------------------------------------: | :---------------------------------------------------: |
| src_to_tkn () | const char* filename | Lexer* | give a filename and open file reads content into buffer and give ownership to Lexer->buffer and return lexer which is created in create_lexer() | caller should responsible for free the returned lexer |
| tokenizer ()  |        Lexer*        | Lexer* | this function give lexer struct and call another function called make_token() which return Token* and make linked list out of this tokens which head of this llist is lexer->tokenlist |                          N/A                          |
| make_token()  |        Lexer*        | Token* | in here we call two funcs inside this function make_token_reserver() and make_token_op() which is clear which is what first one make tokens for reserved keywords (int, char, main, float, . . .) and second is tokenize op's and also char and number literals and keep the value of this tokens |                          N/A                          |

> and also some other helper functions like [is_number, is_char, is_number_or_char, make_token_op and . . . ]

## Lexer struct

```c
sturct _Token{
    const char* filename; 	//borrowed in main 
    char* buffer;			 // owns
    usize line;
    usize column;
    usize pos;
    usize len;
    struct Token* head; 	// pointer to head node token in llist
};
```

 

## Token struct 

```c
strcut _Tokne{
    token_type type;	 // type of the token (TKN_INT, TKN_CHAR, TKN_EOF, . . .)
	void* value;		 // token literal for int or char values (or *pointers)
    struct _Token* next; // pointer to next token for llist 
};
```



## token_type enum

```c
typedef enum {
    TKN_INT,
    TKN_CHAR,
    TKN_MAIN,

    TKN_L_CBRACK, // {
    TKN_R_CBRACK, // {
    TKN_L_PRAN,   // (
    TKN_R_PRAN,   // (

    TKN_EOF,      // end of file
}token_type;
```



