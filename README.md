# (Spring 2020 CPSC 323) C-Compiler - Iteration 02

<ul>
<li>Group Member(s)
	<ul><li><b>Kenneth Doan</b></li>
    <ul><li>snarbolax@csu.fullerton.edu</li></ul>
    </ul>
    </li>

<li>Language(s)
	<ul><li>C</li></ul>
    </li>
    
<li>Assignment Date
	<ul><li>2020/04/16</li></ul>
</li>

<li>Professor
	<ul><li>Prof. Anthony Le, CSU Fullerton</li></ul>
	</li>
</ul>

<ol><li><b>Problem Statement</b></li>
“Write a lexer() function that returns a token when it is needed. The  lexer() should return a record, one field for the token and another field the actual "value" of the token (lexeme).”
<br></br>
"The syntax analyzer should print out the tokens, lexemes, and production rules to an output file. The grammar of the production rules should also be rewritten to remove any left recursion."
<br></br>

<li><b>How to use your program</b></li>
Input “./Lexical [INPUT] [OUTPUT]” into your OS’s terminal while in the same directory/folder of the Lexical program. Replace [INPUT] and [OUTPUT] with the names of the input and output files respectively; an input file and output file are both necessary.
<br></br>

<li><b>Design of your program</b></li>
The program is coded in C and ultimately uses the stdio.h, stdlib.h, string.h, and ctype.h libraries for its implementation. Also, it has 3 functions implemented in it: the print_header function just prints the 

	“TOKENS        Lexemes”
        
line, the lexer function is where the characters and tokens of the input file are parsed, and the main function’s only real purpose is to open&close files and to call the lexer function.  The lexer function takes 2 arguments: the input file and output file; this is already handled by the main function’s calling of the lexer function, as the main function takes in the inputs from the terminal as the arguments.

The program aims to follow the design of a FSM and DFA, where each particular state in each particular situation can only lead to one transition.

It primarily transitions through a 2D int array to determine how it handles each character, depending on the current character that is being read and possibly previous characters that have been read. This is done with the lexer reading each character in the input file one-by-one, though this rule is technically broken when it “reads ahead” in order to try to determine whether the token is possibly a float or a compound operator.
L = letter, D = digit, S = separator, O = operator, SP = space

* Identifier RE: ((L+)(D*)) (S | O | SP)
	* Must start with a letter, with more letters being optional. 
	* Digits are optional. 
Identifier token ends after a separator, operator, or space.
* Real RE: ((D+)( ‘.’ )(D+)) (L | S | O | SP)
	* Must start with a digit. A precision indicator “.” is required somewhere in the middle of the real, with more digits being optional before the precision indicator.
	* At least one other digit is necessary after the precision indicator.
	* The real token ends after a letter, separator, operator, or space.
* Integer RE: (D+) (L | S | O | SP)
	* Must start with a digit, with more digits being optional.
	* Integer token ends after a letter, separator, operator, or space.
<br></br>

~~Also, I have no idea what “YOU MUST ... show the NFSM using Thompson” means.~~ (Rectified in the 2021 C++ iteration of the compiler.)

The syntax analyzer uses the Recursive Descent Parser (RDP) to analyze statements. It relies on Lexical.c to identify tokens and build lists of the identified tokens (char toks[]) and lexemes (char lexs[]) for Syntactical.c to parse through. The built lists are how Lexical.c passes the identified tokens and lexemes to Syntactical.c.

A global iterator (int improv) is used to keep sync and access the different tokens going through each function.

In addition, each rule has been turned into a function which accepts inputs of strings (i.e. tokens) as arguments.  If a token / statement turns out to not be part of a particular rule or statement-type, the function that it is currently in returns a FALSE. Otherwise, the functions will return TRUE and allow the parent functions to continue on parsing the tokens in a statement.

If there are any syntax errors (because of the users) or rule error (due to my implementation of the rules), the statement function (which identifies whether a given statement is a declaration, expression, or assignment) passes the boolean FALSE value to a small error notifier. The notifier outputs the first (likely) token that may be the cause of the syntax error. The error messages from the notifier are only printed to the console and NOT printed to the output file.

The outputs from Lexical.c have been suppressed by default. You may unsuppress them and allow them to print to the console and output file by changing switch_lex_print(false) to switch_lex_print(true) in the main function of Syntactical.c.
<br></br>

<li><b>Any limitations</b></li>
Any possible valid token must be under the size of 512 bytes. The lists for both tokens and lexemes are limited to 512 bytes. The user must change the definition of BUFLEN -- which is located near the top of the source file -- to an integer value larger than 512 if they want to manually bypass this limitation.
<br></br>

<li><b>Any shortcomings</b></li>
<ul><li>The lexer is not smart enough to disqualify floats that have multiple precision indicators “.” from being floats.</li>
	<li>The 2D array does not include all of the possible states and transitions for each situation (i.e. a handful of exceptions are in loops instead of being implemented as a state in the array.)</li>
	<li>The rules for parsing compound operators are not implemented correctly in Lexical.c, as they are discarded as invalid tokens instead.</li>
    <li>The "if" and "while" rules are not implemented.</li></ul>
</ol>