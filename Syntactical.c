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
//#include "Lexical.c"

int it2 = 0; // used as a base to move through toks and lexs
bool new_statement = true; 
bool syn_print = true; // allows fprintf and printf statements to execute. Default is true.
bool test = false; // allows found() to be executed in every applicable rule. Dependent on syn_print.

void found(void); // prints current token and lexeme found
bool ID(char* identifier);
bool type(char* type_string);
bool declarative(char* d);
bool emptySet(char* e);
bool term_prime(char* tp);
bool term(char* t);
bool expression_prime(char* ep);
bool expression(char* i);
bool factor(char* f);
bool assignment(char* a);
bool statement(char* str);
void syntaxical(char* i);

char prev[BUFLEN]; // holds "current" token to be compared with the tokens used in lower functions. For helping with identifying new tokens.
int improv = 0; // int iterator to parse through list of tokens and lexemes.
int limit = 0; // limit decreases (via incrementing) by one each cycle. Used in improv_guard. Might be useless after seeing some recent testing.

bool newtok = true; // default is true to init for the first token. Recommended to not change this initial value.
bool init_err = true; // default is true for first error. Tags start and likely culprit of the syntax error.

void switch_syn_print(bool i) { // allows fprintf and printf statements to execute
	syn_print = i;
}
void found_newtok() { // I really don't remember the logic of how I implemented this and why I placed newtok statements in particular locations.
	if (newtok == true) {
		if (improv == 0) { found(); }
		else {
			if (strcmp(prev, get_lex(improv)) != 0)
				found();
		}
		newtok = false;
	}
	else if (newtok == false) { // I actually don't know how this actually helped, but it did.
			//strcpy(prev, i);
			if (strcmp(prev, get_lex(improv)) != 0)
				found();
		newtok = false;
	}
}

void take_prev(char* i) { // moves "current" token into prev, which is to be compared with the tokens used in lower functions.
	strcpy(prev, i);
}

bool improv_guard(void) {
	improv++;
	if (improv >= list_count-limit) { // if next token is not available, according to the count of elements for lists, move back iterator and return false;
		improv--;
		return false;
	}
	return true;
}

void found(void) {
	fprintf(fileOUT, "\nToken: %s\t\tLexeme: %s\n", get_tok(it2+improv), get_lex(it2 +improv));
	printf("\nToken: %s\t\tLexeme: %s\n", get_tok(it2 +improv), get_lex(it2 +improv));
}

bool type(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		//if (strcmp(get_tok(it2 + improv), "KEYWORD") == 0) {
			fprintf(fileOUT, "<Type> -> int | bool | float\n");
			printf("<Type> -> int | bool | float\n");
		//}
	}
	if (strcmp(get_lex(it2 + improv), i) == 0) { // double checks if lexeme matches, for writer error
		if (strcmp(get_tok(it2 + improv), "KEYWORD") == 0) {
			if (strcmp(get_lex(it2 + improv), "int") == 0) {
				newtok = true;
				return true;
			}
			else if (strcmp(get_lex(it2 + improv), "float") == 0) {
				newtok = true;
				return true;
			}
			else if (strcmp(get_lex(it2 + improv), "bool") == 0) { // only recognizes int, float, bool data types, as other data types are not recognized and supported by Lexical.c yet.
				newtok = true;
				return true;
			}
		}
	}
	return false;
}

bool declarative(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		fprintf(fileOUT, "<Declarative> -> <Type> <Identifier> ; | <Type> < Identifier>\n");
		printf("<Declarative> -> <Type> <Identifier> ; | <Type> < Identifier>\n");
	}
	if (type(get_lex((it2)+improv))) {

		if (improv_guard() == false)
			return false;

		if (ID(get_lex((it2) + improv))) {

			if (improv_guard() != false) {
				if (strcmp(get_lex((it2)+improv), ";") == 0) {
					//new_statement = true;
					//found();
					//printf("improv: %d\n", improv);
					//printf("lex: %s\n", get_lex(improv));
					//newtok = true;
					return emptySet(get_lex(it2+improv));
				}
				else
					--improv;
			}
				return true;
		}
	}

	return false;
}

bool assignment(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		fprintf(fileOUT, "<Assign> -> <Identifier> = <Expression> ; | <Identifier> = <Expression>\n");
		printf("<Assign> -> <Identifier> = <Expression> ; | <Identifier> = <Expression>\n");
	}

	if (ID(get_lex((it2)+improv))) {

		if (improv_guard() == false)
			return false;

		if (strcmp(get_lex((it2) + improv), "=") == 0) {
			//newtok = true;
			//found_newtok();
			if(syn_print == true)
				found();

			if (improv_guard() == false)
				return false;

			if (expression(get_lex((it2) + improv))) {

				if (improv_guard() != false) {
					if (strcmp(get_lex((it2)+improv), ";") == 0) {
						new_statement = true;
						newtok = true;
						return emptySet(get_lex(it2+improv));
					}
					else
						--improv;
				}
				return true;
			}
		}
	}
	return false;
}

bool statement(char* i) {
	int orig = improv;
	if (syn_print == true) {
		if (test == true)
			found();
		//if (new_statement == true) {
		found_newtok();
		take_prev(i);
		if (new_statement == true) {
			fprintf(fileOUT, "<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While> \n");
			printf("<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While> \n");
		}
	}
	switch_syn_print(false);
	if (declarative(get_lex(improv)) == true)
	{
		switch_syn_print(true);
		improv = orig;
		declarative(i);
		switch_syn_print(false);
		return true;
	}
	else if (assignment(get_lex(improv)) == true) {
		switch_syn_print(true);
		/*if (new_statement == true) {
			fprintf(fileOUT, "<Statement> -> <Assign>\n");
			printf("<Statement> -> <Assign>\n");
		}*/
		improv = orig;
		assignment(i);
		switch_syn_print(false);
		return true;
	}
	else if (expression(get_lex(improv)) == true) {
		switch_syn_print(true);
		improv = orig;
		expression(i);
		switch_syn_print(false);
		return true;
	}
	else {
		switch_syn_print(true);
		found();
		switch_syn_print(false);
		return true;
	}

	return false;
}




bool emptySet(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		fprintf(fileOUT, "<Empty> -> Epsilon\n");
		printf("<Empty> -> Epsilon\n");
	}
	if (strcmp(get_lex((it2)+improv), ";") == 0) {
		//syn_print = true;
		new_statement = true;
	}
	else if (strcmp(get_lex(it2+improv), "*") == 0 || strcmp(get_lex(it2 + improv), "/") == 0) {
		if (term_prime(get_lex(it2 + improv)))
			return true;
		return false;
	}
	else if (strcmp(get_lex(it2 + improv), "+") == 0 || strcmp(get_lex(it2 + improv), "-") == 0) {
		if (expression_prime(get_lex(it2 + improv)))
			return true;
		return false;
	}

	newtok = true;
	return true;
}

bool expression(char* i) {
	if (new_statement == true) {
		new_statement = false;
		if (syn_print == true) {
			if (test == true)
				found();
			found_newtok();
			take_prev(i);
			fprintf(fileOUT, "<Expression> -> <Term> <ExpressionPrime>\n");
			printf("<Expression> -> <Term> <ExpressionPrime>\n");
		}

		if (term(get_lex((it2)+improv))) {
			if (improv_guard() == false)
				return false;
			if (expression_prime(get_lex((it2)+improv)))
				return true;
		}
	}
	else {
		//new_statement = false;
		return expression_prime(get_lex((it2)+improv));
	}

	return false;
}

bool expression_prime(char* i) {
	if (new_statement == false) {
		new_statement = false;
		if (syn_print == true) {
			if (test == true)
				found();
			found_newtok();
			take_prev(i);
			fprintf(fileOUT, "<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Term> | <Empty>\n");
			printf("<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Term> | <Empty>\n");
		}

		if (strcmp(get_lex((it2)+improv), "+") == 0) {

			if (improv_guard() == false)
				return false;

			if (term(get_lex((it2)+improv))) {

				if (improv_guard() == false)
					return false;

				if (expression_prime(get_lex((it2)+improv)))
				{
					return true;
				}
			}
		}
		else if (strcmp(get_lex((it2)+improv), "-") == 0) {

			if (improv_guard() == false)
				return false;

			if (term(get_lex((it2)+improv))) {

				if (improv_guard() == false)
					return false;

				if (expression_prime(get_lex((it2)+improv)))
				{
					return true;
				}
			}
		}
		else if (term(get_lex((it2)+improv)))
			return true;
		else if (emptySet(get_lex((it2)+improv)))
			return true;
		//return false;
	}
	else {
		//new_statement = false;
		return expression(get_lex((it2)+improv));
	}
	return false;
}

bool term(char* i) {
	if (new_statement == true) {
		new_statement = false;
		if (syn_print == true) {
			if (test == true)
				found();
			found_newtok();
			take_prev(i);
			fprintf(fileOUT, "<Term> -> <Factor> <TermPrime>\n");
			printf("<Term> -> <Factor> <TermPrime>\n");
		}

		if (factor(get_lex((it2)+improv))) {

			if (improv_guard() == false)
				return false;

			if (term_prime(get_lex((it2)+improv)))
				return true;
		}
	}
	else {
		//new_statement = false;
		return term_prime(get_lex((it2)+improv));
	}

	return false;
}

bool term_prime(char* i) {
	if (new_statement == false){
		new_statement = false;
		if (syn_print == true) {
			if (test == true)
				found();
			found_newtok();
			take_prev(i);
			fprintf(fileOUT, "<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Factor> | <Empty>\n");
			printf("<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Factor> | <Empty>\n");
		}

		if (strcmp(i, "*") == 0) {

			if (improv_guard() == false)
				return false;

			if (factor(get_lex((it2)+improv))) {

				if (improv_guard() == false)
					return false;

				if (term_prime(get_lex((it2)+improv)))
					return true;
			}
		}
		else if (strcmp(i, "/") == 0) {

			if (improv_guard() == false)
				return false;

			if (factor(get_lex((it2)+improv))) {

				if (improv_guard() == false)
					return false;

				if (term_prime(get_lex((it2)+improv)))
					return true;
			}
		}
		else if (factor(i))
			return true;
		else if (emptySet(i))
			return true;
	}
	else {
		//new_statement = false;
		return term(get_lex((it2)+improv));
	}

	return false;
}

bool factor(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		fprintf(fileOUT, "<Factor> -> ( <Expression> ) | <Identifier> | <Num>\n");
		printf("<Factor> -> ( <Expression> ) | <Identifier> | <Num>\n");
	}

	if ((strcmp(get_lex((it2)+improv), "(") == 0) && (strcmp(get_tok(it2+improv), "SEPARATOR") == 0)) {

		if (improv_guard() == false)
			return false;

		if (expression(get_lex((it2) + improv))) {

			if (improv_guard() == false)
				return false;

			if ((strcmp(get_lex((it2)+improv), ")") == 0) && (strcmp(get_tok(it2 + improv), "SEPARATOR") == 0))
				emptySet(get_lex((it2)+improv));//return true;
		}
	}
	else if (ID(i))
		return true;
	return false;
}

bool ID(char* i) {
	if (syn_print == true) {
		if (test == true)
			found();
		found_newtok();
		take_prev(i);
		//if (strcmp(get_tok(it2 + improv), "IDENTIFIER") == 0) {
			fprintf(fileOUT, "<Identifier> -> id\n");
			printf("<Identifier> -> id\n");
		//}
	}

	if (strcmp(get_tok(it2 + improv), "IDENTIFIER") == 0) { // double checks if token matches
		if (strcmp(get_lex(it2 + improv), i) == 0) { // checks if lexeme matchers
			newtok = true;
			return true;
		}
	}
	return false;
}

void end_info(void) {
	/*
	strcpy(prev, get_lex(improv));
	char* prev2 = get_lex(improv);
	if (prev2 == get_lex(improv))
		printf("\nchar* prev2 == get_lex(improv), TRUE\n");
	if (prev2 == lextemp)
		printf("char* prev2 == lextemp, TRUE\n");
	if (strcmp(prev, get_lex(improv)) == 0)
		printf("strcmp(char prev[BUFLEN], get_lex(improv)) == 0\n");
	if (strcmp(prev, lextemp) == 0)
		printf("strcmp(char prev[BUFLEN], lextemp) == 0\n");
		*/
	printf("Current token at get_token(improv) is %s\n", get_tok(improv));
	printf("Current lexeme at get_lex(improv) is %s\n", get_lex(improv));
	printf("\nCurrent improv is %d\n", improv);
	//printf("Current prev2 is %s\n", prev2);
	printf("Current prev is %s\n", prev);
}

void syntax_error(void) {
	printf("---------------------------------------\nSYNTAX ERROR STARTING AT THIS TOKEN\n");
	lists_check();
	end_info();
	printf("---------------------------------------\n");
}

void syntaxical(char* i) {
	//take_prev(i);
	if (statement(get_lex(improv)) != true) {
		if (init_err == true)
			syntax_error();
		init_err = false;
	}
	/*
	while (improv < list_count-1) {
		improv++;

		//if(improv_guard() == false)
		//	break;
		//limit++; //useless?

		syntaxical(get
		_lex(improv));
	}*/

	while (improv_guard() == true) {
		//limit++; //Don't actually use this.
		syntaxical(get_lex(improv));
		//if ((int_count) == (list_count - 1))
		//{
		//	printf("Finish.\n");
			//printf("tok %s\n", lextemp);
		//}
	}
}

int main(int argc, const char* argv[]) {
	open_files(argv);

	switch_lex_print(false);
	lexer(fileIN, fileOUT);

	//lists_check();

	switch_syn_print(true);
	syntaxical(get_lex(improv));
	fprintf(fileOUT, "Finish.\n");
	printf("Finish.\n");

	//end_info();

	close_files(fileIN, fileOUT);

	return 0;
}
