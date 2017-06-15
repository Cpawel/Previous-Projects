/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TKstate{integer, zero, octal, hex, malformed, floating, reset, floating2, floating3};// state of currently input item. Floating2 denotes that an E was already found, floating3 is for +/-.
enum TKstate state = reset; //global state variable
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char * str; // string storage
	int ptr; // pointer to current position within string, to allow for bit by bit exploration
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
	int strsize = strlen(ts);//size of input string
	if(strsize == 0){ // Lack of items in string
		return NULL;
	}
	TokenizerT *tk = (TokenizerT *) malloc(sizeof(TokenizerT));
	tk->str = (char *)malloc(strsize + 1); //add one to allow for null character when using strcpy
	strcpy(tk->str, ts);
	tk->ptr = 0;
	return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	free(tk->str);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk) {
	/* returns the token, but an extra parameter was set inside of TKGetNextToken in order to output the state
	 * never fails, due to checks outside this method, thus 0 is never returned */

	int holder = tk->ptr;// holds position of ptr
	int length = 0;
	char *ret; // item to return
	char *token = (char *) malloc(sizeof(tk->str + 1));// token that is created by individually appending characters
	while( tk->str[holder] != '\0'){

		if(state == malformed){// has reached an end state of malformed
			ret = (char *) malloc((length) * sizeof(char));
			token[length] = '\0';
			int i;
			for(i = 0; i <= length; i++)
				ret[i] = token[i];
			tk->ptr = holder; // update tokenizer pointer, preparing for next token set
			free(token);
			return ret;
		}

		if(tk->str[holder] == '0'){ //states are updated in the following cascade
			if(state == reset)
				state = zero;
			else if(state == zero)
				state = octal;

		}else if(isdigit(tk->str[holder]) != 0){
			if(state == reset)
				state = integer;
			else if((state == zero || state == octal)){
				if(tk->str[holder] == '8' && tk->str[holder] == '9')
					state = malformed;
				else
					state = octal;
			}else if(state == floating2)//skipped +/-
				state = floating3;

		}else if(tk->str[holder] == 'X' || tk->str[holder] == 'x'){
			if(state == zero)
				state = hex;
			else
				state = malformed;

		}else if(tk->str[holder] == '.'){
			if(state == integer || state == zero || state == reset)
				state = floating;
			else
				state = malformed;

		}else if(isxdigit(tk->str[holder]) != 0){ // item is 0 -> f, but 0-9 have been ruled out so only a->f
			if(state != hex && (tk->str[holder] == 'E' || tk->str[holder] == 'e')){
				if(state == floating || state == integer)
					state = floating2;
				else
					state = malformed;
			}else if(state == hex){
			}else
				state = malformed;

		}else if(tk->str[holder] == '+' || tk->str[holder] == '-'){
			if(state == floating2)
				state = floating3;
			else
				state = malformed;

		}else{//item is an invalid character. Must print previous token along with this character.

			if(state != reset){//This spits out the previous token
				ret = (char *) malloc((length) * sizeof(char));
				token[length] = '\0';
				int i;
				for(i = 0; i <= length; i++)
					ret[i] = token[i];
				tk->ptr = holder;
				return ret;
			}
			if(tk->str[holder] !=' '){ // prints invalid characters
				printf("Invalid character found: [0x%02X] \n", tk->str[holder]);
				holder++;
				ret = malloc(sizeof(char));
				ret[0] = ',';
				free(token);
				tk->ptr = holder;
				return ret;
			}
		}
		if(tk->str[holder] != ' '){// skips spaces when they come. the rest is taken care of by the above
			token[length] = tk->str[holder];
			length++;
		}
		holder++;
	}
	// end of tokens
	if(length != 0){
		ret = (char *) malloc((length) * sizeof(char));
		token[length] = '\0';
		int i;
		for(i = 0; i <= length; i++)
			ret[i] = token[i];
		tk->ptr = holder;
		free(token);
		return ret;
	}else{
		ret = malloc(sizeof(char));
		ret[0] = '\0';
		free(token);
		return ret;
	}
}


/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	TokenizerT *tokenizer = TKCreate(argv[1]);
	if(tokenizer == NULL){
		return 1;
	}
	char *tk = TKGetNextToken(tokenizer); // token gotten from tokenizer to start process
	do{
		if(*tk != ','){
			printState();
			printf(" %s \n", tk);
		}
		tk = TKGetNextToken(tokenizer);
	}while(tk != NULL && *tk != '\0');
	TKDestroy(tokenizer);// Finished with tokenizer

	return 0;
}

int printState(){

	if(state == integer){
		printf("integer  ");
	}else if(state == floating || state == floating2 || state == floating3){
		printf("float  ");
	}else if(state == zero){
		printf("zero  ");
	}else if(state == hex){
		printf("hex  ");
	}else if(state == octal){
		printf("octal  ");
	}else if(state == reset){
		printf("reset  ");// used for testing, should not happen
	}else if(state == malformed){
		printf("malformed  ");
	}else
		printf("unknown state"); //also should not happen
	state = reset;
	return 0;
}

