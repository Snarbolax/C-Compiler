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

#include "compilerhead.h"

internal_count = 0; // toks
internal_c2 = 0; // lexs

char temp[BUFLEN] = { ' ' };

bool lex_print = true; // allows fprintf and printf statements to execute. Default is true.

char separators[] = "'(){}[],.:;";
char operators[] = "*+-=/><%";

int transition = 1;
int iterator = 0;
int current_state = 1;
int EOF_check = 0;
int float_indicator = 0;

//char* pointer;
char c;

fpos_t pos;

list_count = 0;
                                                              // L = letter, D = digit, S = separator, O = operator, SP = space, P = punctuation
					    /* L   D   !   .   S   O  SP   P  Back-up */
int states[12][10] = {{ 0, 0,  0,  0,  0,  0,  0,  0,  0, 90 },  //  0. Empty row for easy formatting. Ignore.

										 /*10*/
					 {  0, 2,  4,  6,  8,  8,  8,  1,  9, 90 },  //  1. Starting State
					 {  0, 2,  2,  3,  3,  3,  3,  3,  3, 90 },  //  2. In identifier
					 {  0, 1,  1,  1,  1,  1,  1,  1,  1, 100 }, //  3. End of identifier
					 {  0, 5,  4,  5,  4,  5,  5,  5,  5, 90 },  //  4. In number
					 {  0, 1,  1,  1,  1,  1,  1,  1,  1, 100 }, //  5. End of number
					 {  0, 6,  6,  7,  6,  6,  6,  6,  6, 90 },  //  6. In !! comment
					 {  0, 1,  1,  1,  1,  1,  1,  1,  1, 95 },  //  7. End of !! comment
					 {  0, 1,  1,  1,  1,  1,  1,  1,  1, 95 },  //  8. General punctuation. Mostly used for standalone separators
                     {  0, 1,  1,  1,  1,  1,  1,  1,  1, 100 }, //  9. General punctuation. Mostly for control characters
                     {  0,11, 11, 11, 11, 11, 11, 11, 11, 90 },  // 10. In possible compound operator
                     {  0, 1,  1,  1,  1,  1,  1,  1,  1, 100 }};// 11. End of possible compound operator
															  // 90 = Blank, 95 = N, 100 = Y
                                                              // End States: 1, 3, 5, 7, 8, 9

															  //#define SIZE(x) (sizeof(x)/sizeof(*(x))) // (array size /array element size) = number of elements in the array for a type of array

void switch_lex_print(bool i) { // allows fprintf and printf statements to execute
	lex_print = i;
}
void set_counts(int a, int b) {
	internal_count = a;
	internal_c2 = b;
}
void lists_check(void) {
	printf("\nAll tokens: %s\n", toks);
	printf("All lexes: %s\n", lexs);
	printf("Number of elements in both lists: %d\n\n", list_count);
}
char* _strtok(char* str, char delimiter, int* internal) { // internal is meant for either internal_conunt or internal_c2;
	char bo[512];
	memset(bo, NULL, sizeof(bo));
	int co = *internal;
	int boco = 0;
	char p;
	p = str[co];

	if (p == NULL)
		return NULL;
	else {
		while (p == delimiter && (p != NULL || p != '\0')) {
			++co;
			p = str[co];
		}
		while (p != delimiter && (p != NULL /*|| *p != '\0'*/)) {
			bo[boco] = p;
			++boco;
			++co;
			p = str[co];
		}
		if (p != '\0' && (p != NULL)) {
			bo[boco] = p;
			++boco;
			++co;
			p = str[co];
		}
	}

	*internal = co;
	//str[*internal] = '\0';

	//printf("Check: %s\n", bo);
	return bo;

}
void get_part(/*char* token, char* lexeme, char clexeme*/void) {
	memset(toktemp, NULL, sizeof(toktemp));
	memset(lextemp, NULL, sizeof(lextemp));
	strcpy(toktemp, _strtok(toks, ' ', &internal_count));
	strcpy(lextemp, _strtok(lexs, ' ', &internal_c2));

	toktemp[strlen(toktemp) - 1] = '\0';
	//if (clexeme == NULL)
	lextemp[strlen(lextemp) - 1] = '\0';

	/* if((token != NULL && lexeme != NULL) || (token != NULL && clexeme != NULL))
	if (_debug) {
		if (strcmp(toktemp, token) == 0)
			printf("Tok Check: %s\n", toktemp);
		else
			printf("Error: Token does not match.\n");

		if (clexeme == NULL) {
			if (strcmp(lextemp, lexeme) == 0)
				printf("Lex Check: %s\n\n", lextemp);
			else
				printf("Error: String Lexeme does not match.\n");
		}
		else if (lexeme == NULL) {
			char ion;
			ion = lextemp[0];
			if (ion == clexeme)
				printf("Lex Check: %s\n\n", lextemp);
			else
				printf("Error: Char Lexeme does not match.\n");
		}
	}*/
}
char* get_tok(int move) {
	for (int i = 0; i < (move+1); ++i) {
		memset(toktemp, NULL, sizeof(toktemp));
		strcpy(toktemp, _strtok(toks, ' ', &internal_count));
	}

	toktemp[strlen(toktemp) - 1] = '\0';

	internal_count = 0;
	return toktemp;
}
char* get_lex(int move) { // `move` is the index of where you want to move to. i.e. move = 0, means index 0. move = 3, means index 3.
	if (move >= list_count)
		fprintf(stderr, "Error: Given index is greater than number of actual elements in the lists.\nMax given argument is expected to be List-Size - 1.\nList Size: %d\nMax index / expected-argument: %d\nGiven argument: %d\n", list_count, (list_count-1), move);
	for (int i = 0; i < (move+1); ++i) {
		memset(lextemp, NULL, sizeof(lextemp));
		strcpy(lextemp, _strtok(lexs, ' ', &internal_c2));
	}

	lextemp[strlen(lextemp) - 1] = '\0';

	internal_c2 = 0;
	return lextemp;
}
char get_clex(int move) {
	for (int i = 0; i < (move+1); ++i) {
		memset(lextemp, NULL, sizeof(lextemp));
		strcpy(lextemp, _strtok(lexs, ' ', &internal_c2));
	}

	lextemp[strlen(lextemp) - 1] = '\0';

	char ion;
	ion = lextemp[0];

	internal_c2 = 0;
	return ion;
}

void build_lists(char* t, char* l) { // char * strdup(const char *str1); in string.h ?
	strcat(toks, t);
	strcat(toks, " ");

	strcat(lexs, l);
	strcat(lexs, " ");

	++list_count;

	//get_part(t, l, NULL);
}
void build_clists(char* t, char l) {
	strcat(toks, t);
	strcat(toks, " ");

	lexs[strlen(lexs)] = l;
	strcat(lexs, " ");

	++list_count;

	//get_part(t, NULL, l);
}

void open_files(char** argv) {
	fileIN = fopen(*(argv+1), "r");
	fileOUT = fopen(*(argv+2), "w");
}
void close_files(FILE* IN, FILE* OUT) {
	fclose(IN);
	fclose(OUT);
}

void print_header(void){
	fprintf(fileOUT, "TOKENS\t \tLexemes\n\n");
	printf("TOKENS\t \t\tLexemes\n\n");
}


void lexer(FILE* fin, FILE* fout) {
	if (lex_print == true)
		print_header();
	c = fgetc(fin);

	while (EOF_check != 1) {
		if (c == '!') {
			transition = 3;
		}
		else if (isspace(c)) {
			transition = 7;
		}
		//else if(iscntrl(c)){
		//    transition = 8;
		//}
		else if (isalpha(c)) {
			transition = 1;
			temp[iterator] = c;
		}
		else if (isdigit(c)) {
			transition = 2;
			temp[iterator] = c;

			fgetpos(fin, &pos); // position of the current digit in stream
			c = fgetc(fin); // checks if next position is the '.' separator
			if (c == '.') {
				c = fgetc(fin); // checks if the position after the '.' separator is a digit.
				if (isdigit(c)) {
					float_indicator = 1; // now smart enough to notice 5.9 is a float.
					// Not smart enough to disqualify 5.9.6 or 5.99.677.2 as a float
				}
			}
			fsetpos(fin, &pos); // sets the position indicator back to the original '.'
		}
		else if (strpbrk(&c, separators) != NULL) {
			if (float_indicator == 1) {
				transition = 4;
				temp[iterator] = c;
				float_indicator = 0;
			}
			else
				transition = 5;
		}
		else if (strpbrk(&c, operators) != NULL) {
			/*if (strpbrk(&c, '=') != NULL) {
			 current_state = 10;
			 temp[iterator] = c;
			 }*/
			 /*if (strpbrk(&c, "+-/*%<>") != NULL) {
				 char compound_cand = c;
				 fgetpos(fin, &pos);
				 c = fgetc(fin);
				 if (c == '=' && iterator == 0) {
					 temp[iterator++] = compound_cand;
					 temp[iterator] = c;
					 current_state = 10;
				 }
				 else
					 fsetpos(fin, &pos);
			 }*/
			transition = 6;
		}
		current_state = states[current_state][transition];

		if (current_state != 6) {
			if (current_state == 5) // End of number
			{
				if (strpbrk(temp, ".") == NULL) // no . precision found in the number means the number is an integer
				{
					if (lex_print == true) {
						printf("INTEGER\t\t=\t\%s\n", temp);
						fprintf(fout, "INTEGER\t\t=\t\%s\n", temp);
					}
					build_lists("INTEGER", temp);
				}
				else if (strpbrk(temp, ".") != NULL) // precision found in the number
				{
					if (lex_print == true) {
						printf("REAL\t\t=\t\%s\n", temp);
						fprintf(fout, "REAL\t=\t\%s\n", temp);
					}
					build_lists("REAL", temp);
				}
			}
			else if (current_state == 3) // End of identifier
			{
				if (strcmp("int", temp) == 0 || strcmp(temp, "float") == 0 || strcmp(temp, "bool") == 0 || strcmp(temp, "if") == 0 ||
					strcmp(temp, "else") == 0 || strcmp(temp, "then") == 0 || strcmp(temp, "endif") == 0 || strcmp(temp, "while") == 0 ||
					strcmp(temp, "whileend") == 0 || strcmp(temp, "do") == 0 || strcmp(temp, "doend") == 0 || strcmp(temp, "for") == 0 ||
					strcmp(temp, "forend") == 0 || strcmp(temp, "input") == 0 || strcmp(temp, "output") == 0 || strcmp(temp, "and") == 0 ||
					strcmp(temp, "or") == 0 || strcmp(temp, "not") == 0)
				{
					if (lex_print == true) {
						printf("KEYWORD\t\t=\t\%s\n", temp);
						fprintf(fout, "KEYWORD\t\t=\t\%s\n", temp);
					}
					build_lists("KEYWORD", temp);
				}
				else {
					if (lex_print == true) {
						printf("IDENTIFIER\t=\t\%s\n", temp);
						fprintf(fout, "IDENTIFIER\t=\t\%s\n", temp);
					}
					build_lists("IDENTIFIER", temp);
				}
			}
			else if (current_state == 9 && strpbrk(&c, operators) == NULL) {
				if (lex_print == true) {
					printf("OTHER\t=\t\%c\n", c);
					fprintf(fout, "OTHER\t=\t\%c\n", c);
				}
				build_clists("OTHER", c);
				//current_state = states[current_state][transition];
			}

			if ((current_state == 8) && strpbrk(&c, separators) != NULL) //C must be a separator.
			{
				if (lex_print == true) {
					printf("SEPARATOR\t=\t\%c\n", c);
					fprintf(fout, "SEPARATOR\t=\t\%c\n", c);
				}
				build_clists("SEPARATOR", c);
			}
			//current_state == 11
			else if ((current_state == 8) && strpbrk(&c, operators) != NULL) // C must be an operator.
			{
				if (lex_print == true) {
					printf("OPERATORS\t=\t\%c\n", c);
					fprintf(fout, "OPERATORS\t=\t\%c\n", c);
				}
				build_clists("OPERATORS", c);
			} // Don't know how to incorporate compound operators
			else if (current_state == 11) { // compound operator
				if (lex_print == true) {
					printf("OPERATORS\t=\t\%s\n", temp);
					fprintf(fout, "OPERATORS\t=\t\%s\n", temp);
				}
				build_lists("OPERATORS", temp);

				current_state = states[current_state][transition];
				memset(temp, NULL, sizeof(temp));
				iterator = 0;
				c = fgetc(fin);
				continue;
			}
		}

		if (states[current_state][9] == 95 || current_state == 1 | current_state == 9) { // clears temp and move c when it's unnecessary to hold onto current char
			current_state = states[current_state][transition];
			memset(temp, NULL, sizeof(temp));
			iterator = 0;
			c = fgetc(fin);
		}
		else if (states[current_state][9] == 90) { // transistor
			if (current_state != 6 && current_state != 10) // keeps compound operator candidate
				current_state = states[current_state][transition]; // recheck and resolve current_state for next iteration.
			++iterator; // moves along iterator location in temp
			c = fgetc(fin); // moves along current char location in the file
		}
		else if (states[current_state][9] == 100) { // saves current c as next new parse when current state is at 3, 5, 9, or 11
			memset(temp, NULL, sizeof(temp));
			iterator = 0;
			temp[iterator] = c; //put c at start of temp
			current_state = states[current_state][transition]; // resets current_state back to 1. i.e. start a new parse with current c.
		}


		if (c == EOF || feof(fin)) {
			EOF_check = 1;
			break;
		}
	}

	if (current_state != 1 && current_state != 3 && current_state != 5 && current_state != 7 && current_state != 8 && current_state != 9) {
		fprintf(stderr, "Error: Current state is not an end state.\nCurrent State: \%d\n", current_state);
		exit(1);
	}/*
	else if (isblank(c) && (strcmp(get_lex(list_count - 1), ";") != 0)) {
		;
	}*/
	else if (list_count == 0) {
		fprintf(stderr, "Error: List is empty.\n");
		exit(1);
	}
	else if (strcmp(get_lex(list_count-1), ";") != 0) {
		fprintf(stderr, "Error: Last token is not ';'\Last Token: \%s\n", get_lex(list_count-1));
		exit(1);
	}
}

int main(int argc, const char* argv[]) {
	open_files(argv);
	//fileIN = fopen(argv[1], "r");
	//fileOUT = fopen(argv[2], "w");

    lexer(fileIN, fileOUT);
	if (_debug == true)
		lists_check();
	close_files(fileIN, fileOUT);

	return 0;
