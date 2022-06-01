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

<li><b>How to use your program</b></li>
Input “./Lexical [INPUT] [OUTPUT]” into your OS’s terminal while in the same directory/folder of the Lexical program. Replace [INPUT] and [OUTPUT] with the names of the input and output files respectively; an input file and output file are both necessary.
<br></br>

<li><b>Design of your program</b></li>
The program ultimately uses the stdio.h, stdlib.h, string.h, and ctype.h libraries for its implementation. Also, it has 3 functions implemented in it: the print_header function just prints the 

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

~~Also, I have no idea what “YOU MUST ... show the NFSM using Thompson” means.~~

Rectified in the 2021 C++ iteration of the compiler.
<br></br>

<li><b>Any limitations</b></li>
Any possible valid token must be under the size of 100 bytes. The user must change the definition of BUFLEN -- which is located near the top of the source file -- to an integer value larger than 100 if they want to manually bypass this limitation.
<br></br>

<li><b>Any shortcomings</b></li>
The lexer is not smart enough to disqualify floats that have multiple precision indicators “.” from being floats. The 2D array does not include all of the possible states and transitions for each situation (i.e. a handful of exceptions are in loops instead of being implemented as a state in the array.)
</ol>