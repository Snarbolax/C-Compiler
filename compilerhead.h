/*
   Copyright 2020 Kenneth Doan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

//#ifndef compilerhead.h
//#define compilerhead.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BUFLEN 512

FILE* fileIN;
FILE* fileOUT;

//bool _debug = false; // enables printing within some functions

extern int internal_count; // for toks
extern int internal_c2; // for lexs

extern int list_count;

extern void switch_lex_print(bool i);

char toks[BUFLEN]; // holds identified tokens from Lexical.c
char lexs[BUFLEN]; // holds identified lexemes from Lexical.c
char toktemp[BUFLEN];
char lextemp[BUFLEN];

extern void print_header(void);
extern void lexer(FILE* fin, FILE* fout);
extern void syntaxical(char* i);

extern void lists_check(void);

extern char* get_tok(int move);
extern char* get_lex(int move);

extern void open_files(char** argv);
extern void close_files(FILE* IN, FILE* OUT);

//#endif compilerhead.h